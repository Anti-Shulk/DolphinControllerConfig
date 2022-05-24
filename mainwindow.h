#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGamepadManager>
#include <QFrame>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

//    void on_Quit_clicked();

//    QString getControllerProfile();

    void replaceFileText(QString filePath, QString toBeReplaced, QString toReplaceWith);

    void replaceFileLine(QString filePath, QString toBeReplaced, QString toReplaceWith);

    void replaceFileLines(QString filePath, QString toBeReplacedStart, QString toBeReplacedEnd, QString toReplaceWith);

    QString fileToString(QString filePath);

    void resetControllerProfiles();



//    void writeControllerProfileToTempFile(int controllerNumber, QString controllerType);

//    void writeControllerProfile(int controllerNumber, QString controllerType);

    void setAsSelected(QFrame *frame);

    void setAsDeselected(QFrame *frame);

    void setSelectedFrame(int frameNumber);

    void aPressed(bool value);

    void upPressed(bool value);

    void downPressed(bool value);

    void leftPressed(bool value);

    void rightPressed(bool value);

    void saveSettings();

    void loadRealControllers();

    QString getSetting(QString group, QString key);

    QString getPlayerNumber();

    QString getPlayerConfig(int playerNumber, QString key);

    QString getPlayerConfig(QString key);

    void setPlayerConfig(QString key, QString value);

    QString getRealControllerKey(int keyNumber);

    QString getCurrentRealControllerKey();

    QString getRealControllerValue(QString key);

    QString getPath();

    QString getCurrentRealControllerValue();

    void setCurrentRealController();

    void setRealController(QString key);

    QString getEmulatedController();

    void setEmulatedControllerWithoutArrows(int selection);

    void setEmulatedController(int selection);

    void setEmulatedController(QString selection);

    void setCurrentEmulatedController();

    QStringList getProfileSuffixes();

    void setPort(int portNumber);

    void setCurrentPort();


    void setProfile(QString profile);

    bool isRealController();

    void setPlayerNumber();

    void loadCurrentPlayerConfig();

    void selectionAction(bool increasing);

    void launch();


private:
    Ui::MainWindow *ui;
    QFrame *frames[6];
};
#endif // MAINWINDOW_H
