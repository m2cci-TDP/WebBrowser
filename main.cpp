#include "fenprincipale.h"

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  FenPrincipale fenetre;
  fenetre.showMaximized();

  return app.exec();
}
