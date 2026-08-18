#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "configgenerator.h"
#include <QScrollArea>
#include <QApplication>
#include <QButtonGroup>
#include <QCheckBox>
#include <QClipboard>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QRadioButton>
#include <QStyleFactory>
#include <QStringList>
#include <QVBoxLayout>
#include <QDesktopServices>
#include <QUrl>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setDarkTheme();

    m_printerGroup = new QButtonGroup(this);
    m_boardGroup = new QButtonGroup(this);
    m_levelingGroup = new QButtonGroup(this);
    m_displayGroup = new QButtonGroup(this);
    m_thermistorGroup = new QButtonGroup(this);

    populateDynamicSections();
    refreshCommandPreview();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setDarkTheme()
{
    qApp->setStyle(QStyleFactory::create("Fusion"));
    QPalette palette;
    palette.setColor(QPalette::Window, QColor(13, 17, 23));
    palette.setColor(QPalette::WindowText, QColor(201, 209, 217));
    palette.setColor(QPalette::Base, QColor(22, 27, 34));
    palette.setColor(QPalette::AlternateBase, QColor(28, 33, 40));
    palette.setColor(QPalette::ToolTipBase, QColor(22, 27, 34));
    palette.setColor(QPalette::ToolTipText, QColor(201, 209, 217));
    palette.setColor(QPalette::Text, QColor(201, 209, 217));
    palette.setColor(QPalette::Button, QColor(28, 33, 40));
    palette.setColor(QPalette::ButtonText, QColor(201, 209, 217));
    palette.setColor(QPalette::BrightText, QColor(255, 255, 255));
    palette.setColor(QPalette::Highlight, QColor(88, 166, 255));
    palette.setColor(QPalette::HighlightedText, QColor(255, 255, 255));
    palette.setColor(QPalette::Link, QColor(88, 166, 255));
    palette.setColor(QPalette::LinkVisited, QColor(121, 192, 255));
    palette.setColor(QPalette::PlaceholderText, QColor(139, 148, 158));
    qApp->setPalette(palette);

    QString style = R"(
    QGroupBox QCheckBox, QGroupBox QRadioButton {
        spacing: 8px;
        background: none;
        border: none;
    }

    QCheckBox::indicator, QRadioButton::indicator {
        width: 14px;
        height: 14px;
        border: 1px solid #30363d;
        background: #1c2128;
    }

    QCheckBox::indicator:checked, QRadioButton::indicator:checked {
        background: #48ff00;
        border-color: black;
    }
)";

    setStyleSheet(style);
}

//Dynamic Section
void MainWindow::populateDynamicSections()
{
    populateRadioButtons("_printers", m_printerGroup, "Ender3V2");
    populateRadioButtons("_boards", m_boardGroup, "422");
    populateRadioButtons("_leveling", m_levelingGroup, "BLT");
    populateRadioButtons("_displays", m_displayGroup, "DWIN");
    populateRadioButtons("_thermistor", m_thermistorGroup, "T1");
    populateFeatureSection();

    auto wrapInScrollArea = [](QVBoxLayout *targetLayout) -> QVBoxLayout* {
        if (!targetLayout) return nullptr;

        QScrollArea *scrollArea = new QScrollArea();
        scrollArea->setWidgetResizable(true);
        scrollArea->setFrameShape(QFrame::NoFrame);
        scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

        scrollArea->setStyleSheet("QScrollArea { background: transparent; border: none; }");
        scrollArea->setAttribute(Qt::WA_TranslucentBackground);
        scrollArea->viewport()->setStyleSheet("background: transparent;");
        scrollArea->viewport()->setAutoFillBackground(false);

        QWidget *canvas = new QWidget();
        canvas->setStyleSheet("background: transparent;");
        canvas->setAutoFillBackground(false);

        QVBoxLayout *canvasLayout = new QVBoxLayout(canvas);
        canvasLayout->setContentsMargins(8, 8, 8, 8);
        canvasLayout->setSpacing(6);
        canvasLayout->setAlignment(Qt::AlignTop);

        scrollArea->setWidget(canvas);
        targetLayout->addWidget(scrollArea);

        return canvasLayout;
    };

    QVBoxLayout* printerScrollLayout = wrapInScrollArea(ui->printerLayout);
    QVBoxLayout* boardScrollLayout   = wrapInScrollArea(ui->boardLayout);
    QVBoxLayout* levelingScrollLayout= wrapInScrollArea(ui->levelingLayout);
    QVBoxLayout* displayScrollLayout = wrapInScrollArea(ui->displayLayout);
    QVBoxLayout* thermistorScrollLayout = wrapInScrollArea(ui->thermistorLayout);

    for (QAbstractButton *button : m_printerGroup->buttons())    { if (printerScrollLayout) printerScrollLayout->addWidget(button); }
    for (QAbstractButton *button : m_boardGroup->buttons())      { if (boardScrollLayout) boardScrollLayout->addWidget(button); }
    for (QAbstractButton *button : m_levelingGroup->buttons())   { if (levelingScrollLayout) levelingScrollLayout->addWidget(button); }
    for (QAbstractButton *button : m_displayGroup->buttons())    { if (displayScrollLayout) displayScrollLayout->addWidget(button); }
    for (QAbstractButton *button : m_thermistorGroup->buttons()) { if (thermistorScrollLayout) thermistorScrollLayout->addWidget(button); }

    QCheckBox *ublCheck = new QCheckBox("UBL", this);
    ublCheck->setChecked(true);
    m_ublCheck = ublCheck;

    if (levelingScrollLayout) {
        levelingScrollLayout->addWidget(ublCheck);
    } else {
        ui->levelingLayout->addWidget(ublCheck);
    }

    connect(ublCheck, &QCheckBox::toggled, this, &MainWindow::refreshCommandPreview);
    connect(m_levelingGroup, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked), this, &MainWindow::onLevelingChanged);
    connect(m_printerGroup, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked), this, &MainWindow::refreshCommandPreview);
    connect(m_boardGroup, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked), this, &MainWindow::refreshCommandPreview);
    connect(m_displayGroup, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked), this, &MainWindow::refreshCommandPreview);
    connect(m_thermistorGroup, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked), this, &MainWindow::refreshCommandPreview);

    onLevelingChanged();
}

void MainWindow::populateRadioButtons(const QString &folder, QButtonGroup *group, const QString &defaultValue)
{
    const QStringList values = ConfigGenerator::availableValues(folder);
    QStringList filtered = values;
    filtered.removeAll("Common");
    filtered.removeAll("Common_OLD");

    for (const QString &value : filtered) {
        QRadioButton *radio = new QRadioButton(value, this);
        radio->setChecked(value == defaultValue);
        group->addButton(radio);
        group->setId(radio, group->buttons().size() - 1);
    }
}

void MainWindow::populateFeatureSection()
{
    const QStringList features = ConfigGenerator::availableValues("_features");
    for (const QString &feature : features) {
        QCheckBox *box = new QCheckBox(feature, this);
        box->setChecked(false);
        ui->featureLayout->addWidget(box);
        m_featureCheckboxes.insert(feature, box);
        connect(box, &QCheckBox::toggled, this, &MainWindow::refreshCommandPreview);
    }
}

QStringList MainWindow::buildConfigurationList() const
{
    QStringList result;

    const QString printer = currentRadioValue(m_printerGroup);
    const QString board = currentRadioValue(m_boardGroup);
    const QString leveling = currentRadioValue(m_levelingGroup);
    const QString display = currentRadioValue(m_displayGroup);
    const QString thermistor = currentRadioValue(m_thermistorGroup);
    if (!printer.isEmpty()) result << printer;
    if (!board.isEmpty()) result << board;
    if (!leveling.isEmpty()) result << leveling;
    if (m_ublCheck && m_ublCheck->isChecked() && leveling != "MM") result << "UBL";
    if (display != "DWIN") result << display;
    if (thermistor != "T1") result << thermistor;

    for (auto it = m_featureCheckboxes.constBegin(); it != m_featureCheckboxes.constEnd(); ++it) {
        if (it.value()->isChecked()) {
            result << it.key();
        }
    }

    return result;
}

QString MainWindow::currentRadioValue(QButtonGroup *group) const
{
    if (!group) return QString();
    QAbstractButton *button = group->checkedButton();
    return button ? button->text() : QString();
}

void MainWindow::refreshCommandPreview()
{
    const QStringList configuration = buildConfigurationList();
    const QString machineName = ui->configNameEdit->text().trimmed();
    QString command = QString("CreateConfigs.Generate('%1',[")
        .arg(machineName.isEmpty() ? "MyConfig" : machineName);

    for (int i = 0; i < configuration.size(); ++i) {
        command += QString("'%1'").arg(configuration.at(i));
        if (i + 1 < configuration.size()) {
            command += ",";
        }
    }
    command += "])";
    ui->commandPreview->setPlainText(command);
}

QString MainWindow::autoNameFromSelection() const
{
    const QStringList config = buildConfigurationList();
    QString name = config.join("-");
    name.replace("-301F", "-F");
    name.replace("F1-BLT", "F1");
    name.replace("F4-BLT", "F4");
    if (!name.contains("F1-UBL") && !name.contains("F4-UBL") && name.contains("UBL")) {
        name.replace("-UBL", "UBL");
    }
    return name;
}

void MainWindow::onLevelingChanged()
{
    const QString leveling = currentRadioValue(m_levelingGroup);
    if (m_ublCheck) {
        const bool enabled = leveling != "MM";
        m_ublCheck->setEnabled(enabled);
        m_ublCheck->setChecked(enabled);
    }
    refreshCommandPreview();
}

void MainWindow::on_autoNameButton_clicked()
{
    ui->configNameEdit->setText(autoNameFromSelection());
    refreshCommandPreview();
}

void MainWindow::on_generateButton_clicked()
{
    const QString machineName = ui->configNameEdit->text().trimmed();
    const QStringList config = buildConfigurationList();

    const bool hadError = ConfigGenerator::Generate(machineName, config);
    if (hadError) {
        QMessageBox::warning(this, "Pro FW", "Generation finished with errors. Check the generated log file in the output folder.");
    } else {
        QMessageBox::information(this, "Pro FW", "Files generated successfully.");
    }
}

void MainWindow::on_copyButton_clicked()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(ui->commandPreview->toPlainText());
    QMessageBox::information(this, "Clipboard", "Configurations copied to clipboard.");
}

void MainWindow::on_configNameEdit_textChanged(const QString &text)
{
    Q_UNUSED(text);
    refreshCommandPreview();
}

void MainWindow::on_githubButton_clicked()
{
    QDesktopServices::openUrl(QUrl("https://github.com/xXHenneBXx"));
}

void MainWindow::on_youtubeButton_clicked()
{
    QDesktopServices::openUrl(QUrl("https://youtube.com/@xXHenneBXx"));
}

void MainWindow::on_discordButton_clicked()
{
    QDesktopServices::openUrl(QUrl("https://discord.gg/RJfyNrVxjD"));
}
