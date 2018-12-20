#ifndef FENPRINCIPALE_H
#define FENPRINCIPALE_H

#define ACCUEIL "http://www.google.fr"

#include <QtWebKit/QtWebKit>
#include <QApplication>
#include <QWidget>
#include <QLayout>
#include <QIcon>
#include <QTabWidget>
#include <QMainWindow>
#include <QMenuBar>
#include <QAction>
#include <QLineEdit>
#include <QToolBar>
#include <QString>
#include <QStatusBar>
#include <QProgressBar>
#include <QWebView>

class FenPrincipale : public QMainWindow
{
  Q_OBJECT

public:
  FenPrincipale();

private:
  void creerActions();
  void creerMenus();
  void creerToolbar();
  void creerStateBar();

  QWebView *pageActuelle() const;
  QWidget *chargerURL(QString url = ACCUEIL);

private slots:
  void changerURL();
  void MiseAJour();
  void nouvelOnglet();
  void fermerOnglet();
  void fermerOnglet(int index);
  void changerNom(const QString & title);
  void precedent();
  void suivant();
  void actualiser();
  void stop();
  void accueil();
  void debutChargement();
  void chargement(int num);
  void finChargement();

private:
  QTabWidget *onglets;

  QAction *actionQuitter;
  QAction *actionNewOnglet;
  QAction *actionCloseOnglet;
  QAction *actionPrecedent;
  QAction *actionSuivant;
  QAction *actionActualiser;
  QAction *actionStop;
  QAction *actionAccueil;

  QMenu *menuFichier;
  QMenu *menuNavigation;

  QLineEdit *nomURL;
  QProgressBar *progressBar;
};

#endif // FENPRINCIPALE_H
