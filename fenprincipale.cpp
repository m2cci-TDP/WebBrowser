#include "fenprincipale.h"

FenPrincipale::FenPrincipale()
{
  creerActions();
  creerMenus();
  creerToolbar();
  creerStateBar();

  // Fenetre
  setMinimumSize(500, 350);
  setWindowIcon(QIcon(":/web"));
  //setCursor(QCursor(QPixmap(":/souris")));
  setWindowTitle(tr("Navigateur"));
  setUnifiedTitleAndToolBarOnMac(true);

  // onglets
  onglets = new QTabWidget(this);
  onglets->setMovable(true);
  onglets->setTabsClosable(true);
  onglets->setTabPosition(QTabWidget::North);
  QObject::connect(onglets, SIGNAL(tabCloseRequested(int)), this, SLOT(fermerOnglet(int)));
  QObject::connect(onglets, SIGNAL(currentChanged(int)), this, SLOT(MiseAJour()));

  // définition de la zone centrale
  onglets->addTab(chargerURL(), "Nouvel onglet");
  setCentralWidget(onglets);
}

QWidget *FenPrincipale::chargerURL(QString url)
{
  QWidget *pageOnglet = new QWidget;
  QVBoxLayout *layout = new QVBoxLayout;
  layout->setContentsMargins(0,0,0,0);

  QWebView *pageWeb = new QWebView;
  pageWeb->load(QUrl(url));

  layout->addWidget(pageWeb);
  pageOnglet->setLayout(layout);

  QObject::connect(pageWeb, SIGNAL(titleChanged(QString)), this, SLOT(changerNom(QString))); // changer les noms titres
  QObject::connect(pageWeb, SIGNAL(loadStarted()), this, SLOT(debutChargement())); // affiche la progress bar
  QObject::connect(pageWeb, SIGNAL(loadProgress(int)), this, SLOT(chargement(int))); // avance la progress bar
  QObject::connect(pageWeb, SIGNAL(loadFinished(bool)), this, SLOT(finChargement())); // supprime la progress bar
  QObject::connect(nomURL, SIGNAL(returnPressed()), this, SLOT(changerURL())); // changer le nom de l'URL

  return pageOnglet;
}

QWebView *FenPrincipale::pageActuelle() const
{
  return onglets->currentWidget()->findChild<QWebView *>();
}

void FenPrincipale::creerActions()
{
  // nouvel onglet
  actionNewOnglet = new QAction(tr("&Nouvel Onglet"), this);
  actionNewOnglet->setShortcut(QKeySequence::AddTab);
  QObject::connect(actionNewOnglet, SIGNAL(triggered(bool)), this, SLOT(nouvelOnglet())); // nouvel onglet

  // fermer onglet
  actionCloseOnglet = new QAction(tr("&Fermer Onglet"), this);
  actionCloseOnglet->setShortcut(QKeySequence::Close);
  QObject::connect(actionCloseOnglet, SIGNAL(triggered(bool)), this, SLOT(fermerOnglet()));

  // quitter
  actionQuitter = new QAction(tr("&Quitter"), this);
  actionQuitter->setShortcut(QKeySequence::Quit);
  QObject::connect(actionQuitter, SIGNAL(triggered(bool)), qApp, SLOT(closeAllWindows())); // quitter

  // précedent
  actionPrecedent = new QAction(QIcon(":/precedent"), tr("&Precedent"), this);
  actionPrecedent->setShortcut(QKeySequence::Back);
  QObject::connect(actionPrecedent, SIGNAL(triggered(bool)), this, SLOT(precedent()));

  // suivant
  actionSuivant = new QAction(QIcon(":/suivant"), tr("&Suivant"), this);
  actionSuivant->setShortcut(QKeySequence::Forward);
  QObject::connect(actionSuivant, SIGNAL(triggered(bool)), this, SLOT(suivant()));

  // actualiser
  actionActualiser = new QAction(QIcon(":/recharge"), tr("Actualise&r"), this);
  actionActualiser->setShortcut(QKeySequence::Refresh);
  QObject::connect(actionActualiser, SIGNAL(triggered(bool)), this, SLOT(actualiser()));

  // stop
  actionStop = new QAction(QIcon(":/stop"), tr("Sto&p"), this);
  QObject::connect(actionStop, SIGNAL(triggered(bool)), this, SLOT(stop()));

  // accueil
  actionAccueil = new QAction(QIcon(":/home"), tr("&Accueil"), this);
  QObject::connect(actionAccueil, SIGNAL(triggered(bool)), this, SLOT(accueil()));
}

void FenPrincipale::creerMenus()
{
  menuFichier = menuBar()->addMenu(tr("&Fichier"));
  menuFichier->addAction(actionNewOnglet);
  menuFichier->addAction(actionCloseOnglet);
  menuFichier->addAction(actionQuitter);

  menuNavigation = menuBar()->addMenu(tr("&Navigation"));
  menuNavigation->addAction(actionPrecedent);
  menuNavigation->addAction(actionSuivant);
  menuNavigation->addAction(actionActualiser);
  menuNavigation->addAction(actionAccueil);

  //QMenu *menuAffichage = menuBar()->addMenu(tr("&Affichage"));
}

void FenPrincipale::changerURL()
{
  QString url = nomURL->text();

  // On rajoute le "http://" s'il n'est pas déjà dans l'adresse
  if (url.left(7) != "http://")
  {
      url = "http://" + url;
      nomURL->setText(url);
  }
  pageActuelle()->load(QUrl(url));
}
void FenPrincipale::MiseAJour()
{
  // changer url
  nomURL->setText(pageActuelle()->url().toString());

  // changer nom
  QString nomLong = pageActuelle()->title();
  if (nomLong.length() > width())
    nomLong = pageActuelle()->title().left(width()) + "...";

  setWindowTitle(nomLong + " - " + tr("Navigateur"));
}

void FenPrincipale::changerNom(const QString & title)
{
  QString nomCourt = title, nomLong = title;
  if (title.length() > 40)
    nomCourt = title.left(40) + "...";
  if (title.length() > width())
    nomLong = title.left(width()) + "...";

  setWindowTitle(nomLong + " - " + tr("Navigateur"));
  onglets->setTabText(onglets->currentIndex(), nomCourt);

  nomURL->setText(pageActuelle()->url().toString());
}

void FenPrincipale::nouvelOnglet()
{
  int index = onglets->addTab(chargerURL(), "Nouvel onglet");
  onglets->setCurrentIndex(index);
}

void FenPrincipale::fermerOnglet()
{
  if (onglets->count() > 1)
  {
    onglets->removeTab(onglets->currentIndex());
  }
  else
  {
    close();
  }
}

void FenPrincipale::fermerOnglet(int index)
{
  if (onglets->count() > 1)
  {
    onglets->removeTab(index);
  }
  else
  {
    close();
  }
}

void FenPrincipale::precedent()
{
  pageActuelle()->back();
}
void FenPrincipale::suivant()
{
  pageActuelle()->forward();
}
void FenPrincipale::actualiser()
{
  pageActuelle()->reload();
}
void FenPrincipale::stop()
{
  pageActuelle()->stop();
}
void FenPrincipale::accueil()
{
  pageActuelle()->load(QUrl(ACCUEIL));
}

void FenPrincipale::creerToolbar()
{
  QToolBar *toolBar = addToolBar(tr("&Navigation"));
  // nom du champ
  nomURL = new QLineEdit;
  nomURL->setDragEnabled(true);

  toolBar->addAction(actionPrecedent);
  toolBar->addAction(actionSuivant);
  toolBar->addAction(actionActualiser);
  toolBar->addAction(actionStop);
  actionStop->setVisible(false);
  toolBar->addAction(actionAccueil);
  toolBar->addWidget(nomURL);

  toolBar->setMovable(true);
  //this->addToolBar(toolBar);
}

void FenPrincipale::creerStateBar()
{
  progressBar = new QProgressBar;
  progressBar->setVisible(false);
  progressBar->setMaximumHeight(14);
  //progressBar->setMinimumWidth(width());
  statusBar()->addWidget(progressBar, 1);
}
void FenPrincipale::debutChargement()
{
  progressBar->setVisible(true);
  actionStop->setVisible(true);
  actionActualiser->setVisible(false);
}
void FenPrincipale::chargement(int num)
{
  progressBar->setValue(num);
}
void FenPrincipale::finChargement()
{
  progressBar->setVisible(false);
  statusBar()->showMessage(tr("Pret"), 2000);
  actionStop->setVisible(false);
  actionActualiser->setVisible(true);
}
