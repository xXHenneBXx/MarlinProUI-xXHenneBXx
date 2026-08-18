#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QButtonGroup>
#include <QCheckBox>
#include <QMainWindow>
#include <QMap>
#include <QStringList>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_autoNameButton_clicked();
    void on_generateButton_clicked();
    void on_copyButton_clicked();
    void on_configNameEdit_textChanged(const QString &text);
    void on_githubButton_clicked();
    void on_youtubeButton_clicked();
    void on_discordButton_clicked();
    void onLevelingChanged();
    void refreshCommandPreview();

private:
    Ui::MainWindow *ui;

    QButtonGroup *m_printerGroup = nullptr;
    QButtonGroup *m_boardGroup = nullptr;
    QButtonGroup *m_levelingGroup = nullptr;
    QButtonGroup *m_displayGroup = nullptr;
    QButtonGroup *m_thermistorGroup = nullptr;
    QMap<QString, QCheckBox *> m_featureCheckboxes;
    QCheckBox *m_ublCheck = nullptr;

    void populateDynamicSections();
    void populateRadioButtons(const QString &folder, QButtonGroup *group, const QString &defaultValue);
    void populateFeatureSection();

    QStringList buildConfigurationList() const;
    QString currentRadioValue(QButtonGroup *group) const;
    QString autoNameFromSelection() const;

    void setDarkTheme();
};

#endif // MAINWINDOW_H
