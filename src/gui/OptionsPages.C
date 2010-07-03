/* MipScope <http://mipscope.sourceforge.net>
 * Copyright (C) 2007 Travis Fischer and Tim O'Donnell
 * 
 * Send bugs, features, ideas to
 * Travis Fischer (tfischer@cs.brown.edu) and Tim O'Donnell (tim@cs.brown.edu)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

/* ---------------------------------------------- *\
   file: OptionsPages.C
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 5/30/2007
\* ---------------------------------------------- */

#include "OptionsPages.H"
#include <QtGui>
#include "Options.H"

/* General:
 *    Toggle verbosity
 *    Toggle Register Aliases or r0...31
 *    Change Default Display Base (2, 8, 10, 16)
 *
 *    Enter path to template file
 *    Show all mips instructions or only frequently used ones in MIPS Reference Pane
 *
 * Editing:
 *    Change font
 *    Toggle syntax popups
 *    Toggle error-highlighting
 *    Toggle auto-indent
 *    Change hard tabs or spaces; and number of spaces to use
 *
 * Debugging:
 *    Enter path to spim/xspim
 *    Change number of lastX instructions gradient (default 2-3)
 *    Toggle editing-on-the-fly  (enable read-only during debugging)
 *    
 *    Future:  Toggle Branch-Delay slot
 */

GeneralOptionsPage::GeneralOptionsPage(QWidget *parent) : QWidget(parent)
{
   QVBoxLayout *mainLayout = new QVBoxLayout;
   QStyle *style = new QCleanlooksStyle();

   // ----------------------------------------------------------
   QGroupBox *group = new QGroupBox(tr("General"));
   QVBoxLayout *l = new QVBoxLayout;
   QCheckBox *ch = new QCheckBox(tr("Verbose output on stderr"));
   ch->setToolTip(tr("Enable/Disable the display of internal debugging information to stderr"));
   ch->setStyle(style);
   connect(ch, SIGNAL(stateChanged(int)), this, SLOT(verbosityChanged(int)));
   ch->setCheckState(Options::verbosity() ? Qt::Checked : Qt::Unchecked);

   l->addWidget(ch);
   ch = new QCheckBox(tr("Use register aliases ($a0, $t1,..)"));
   ch->setToolTip(tr("Display registers via their aliases or as $r1, $r2,.."));
   ch->setStyle(style);
   connect(ch, SIGNAL(stateChanged(int)), this, SLOT(registerAliasesChanged(int)));
   ch->setCheckState(Options::registerAliases() ? Qt::Checked : Qt::Unchecked);
   l->addWidget(ch);

   QLabel *label = new QLabel(tr("Default Display Base:"));
   QComboBox *combo = new QComboBox;
   combo->addItem(tr("Binary (2)"));
   combo->addItem(tr("Octal (8)"));
   combo->addItem(tr("Decimal (10)"));
   combo->addItem(tr("Signed Decimal (10)"));
   combo->addItem(tr("Hex (16)"));
   connect(combo, SIGNAL(currentIndexChanged(int)), this, SLOT(displayBaseChanged(int)));

   int index = 0;
   switch(Options::dislayBase()) {
      case 8:
         index = 1;
         break;
      case 10:
         index = 2;
         break;
      case -10:
         index = 3;
         break;
      case 16:
         index = 4;
         break;
      default:
         break;
   }
   combo->setCurrentIndex(index);
  
   QHBoxLayout *lH = new QHBoxLayout;
   lH->addWidget(label);
   lH->addWidget(combo);
   l->addLayout(lH);

   group->setLayout(l);
   mainLayout->addWidget(group);
   mainLayout->addSpacing(12);

   // ----------------------------------------------------------
   group = new QGroupBox(tr("MIPS Reference Pane"));
   l = new QVBoxLayout();
   QRadioButton *b = new QRadioButton(tr("Show all MIPS r3000/MIPS32 instructions"));
   b->setStyle(style);
   l->addWidget(b);
   b->setChecked(Options::showAllInstructions());
   b = new QRadioButton(tr("Show only the most common instructions"));
   b->setStyle(style);
   b->setChecked(!Options::showAllInstructions());
   l->addWidget(b);
   connect(b, SIGNAL(toggled(bool)), this, SLOT(showAllInstructionsChanged(bool)));
   
   group->setLayout(l);
   mainLayout->addWidget(group);
   mainLayout->addSpacing(12);
   
   // ----------------------------------------------------------
   group = new QGroupBox(tr("Enter Paths"));
   QHBoxLayout *l2 = new QHBoxLayout;
   l = new QVBoxLayout();
   QString toolTip = tr("Enter full path to template file used for new source files.");
   label = new QLabel(tr("Template File:"));
   QLineEdit *edit = new QLineEdit;
   edit->setText(Options::getTemplatePath());
   connect(edit, SIGNAL(textEdited(const QString&)), this, SLOT(templatePathChanged(const QString&)));
   label->setToolTip(toolTip);
   edit->setToolTip(toolTip);
   l2->addWidget(label);
   l2->addWidget(edit);
   l->addLayout(l2);

   toolTip = tr("Enter full path to xspim (optional).");
   l2 = new QHBoxLayout;
   label = new QLabel(tr("xspim:"));
   edit  = new QLineEdit;
   edit->setText(Options::getXSpimPath());
   connect(edit, SIGNAL(textEdited(const QString&)), this, SLOT(xspimPathChanged(const QString&)));
   label->setToolTip(toolTip);
   edit->setToolTip(toolTip);
   l2->addWidget(label);
   l2->addWidget(edit);
   l->addLayout(l2);

   group->setLayout(l);
   mainLayout->addWidget(group);
   mainLayout->addStretch(1);
   // ----------------------------------------------------------

   setLayout(mainLayout);
}

void GeneralOptionsPage::verbosityChanged(int index) {
   Options::m_options->setVerbosity(index != Qt::Unchecked);
}

void GeneralOptionsPage::registerAliasesChanged(int index) {
   Options::m_options->setRegisterAliases(index != Qt::Unchecked);
}

void GeneralOptionsPage::displayBaseChanged(int index) {
   int bases[] = { 2, 8, 10, -10, 16 };
   
   Options::m_options->setDislayBase(bases[index]);
}

void GeneralOptionsPage::showAllInstructionsChanged(bool showImportant) {
   Options::m_options->setShowAllInstructions(!showImportant);
}

void GeneralOptionsPage::templatePathChanged(const QString &str) {
   Options::m_options->setTemplatePath(str);
}

void GeneralOptionsPage::xspimPathChanged(const QString &str) {
   Options::m_options->setXSpimPath(str);
}


EditingOptionsPage::EditingOptionsPage(QWidget *parent) : QWidget(parent)
{
   QVBoxLayout *mainLayout = new QVBoxLayout;
   QStyle *style = new QCleanlooksStyle();

   // ----------------------------------------------------------
   QGroupBox *group = new QGroupBox(tr("Editing"));
   QVBoxLayout *l = new QVBoxLayout;
   QCheckBox *ch = new QCheckBox(tr("Syntax popups"));
   ch->setToolTip(tr("Enable/Disable the display of syntax popups during editing."));
   ch->setStyle(style);
   l->addWidget(ch);
   ch->setCheckState(Options::syntaxPopupsEnabled() ? Qt::Checked : Qt::Unchecked);
   connect(ch, SIGNAL(stateChanged(int)), this, SLOT(syntaxPopupsChanged(int)));

   ch = new QCheckBox(tr("Error highlighting"));
   ch->setToolTip(tr("Enable/Disable on-the-fly error highlighting"));
   ch->setStyle(style);
   l->addWidget(ch);
   ch->setCheckState(Options::errorHighlightingEnabled() ? Qt::Checked : Qt::Unchecked);
   connect(ch, SIGNAL(stateChanged(int)), this, SLOT(errorHighlightingChanged(int)));

   ch = new QCheckBox(tr("Auto-indentation"));
   ch->setToolTip(tr("Enable/Disable attempted auto-indentation on pressing Enter."));
   ch->setStyle(style);
   l->addWidget(ch);
   ch->setCheckState(Options::autoIndentationEnabled() ? Qt::Checked : Qt::Unchecked);
   connect(ch, SIGNAL(stateChanged(int)), this, SLOT(autoIndentationChanged(int)));
   
   QLabel *label  = new QLabel(tr("Set Tabwidth:"));
   QSpinBox *tabs = new QSpinBox;
   tabs->setRange(0, 16);
   tabs->setValue(4); // TODO
   tabs->setSuffix(tr(" spaces"));
   tabs->setSpecialValueText(tr("Disable Tabs"));
   tabs->setValue(Options::tabWidth());
   connect(tabs, SIGNAL(valueChanged(int)), this, SLOT(tabWidthChanged(int)));
   
   QHBoxLayout *lH = new QHBoxLayout;
   lH->addWidget(label);
   lH->addWidget(tabs);
   l->addLayout(lH);

   group->setLayout(l);
   mainLayout->addWidget(group);
   mainLayout->addSpacing(12);
	//-----------------------------------------------------------
	
	group = new QGroupBox(tr("Auto-Save on Run"));
   QVBoxLayout *l2 = new QVBoxLayout;
   QRadioButton *rb1 = new QRadioButton(tr("Prompt to save on run."));
   rb1->setToolTip(tr("If you try to run an unsaved file, you will be prompted to save it."));
   rb1->setStyle(style);
   l2->addWidget(rb1);
   rb1->setChecked(Options::autoSaveOptions() == Options::PROMPT);
   connect(rb1, SIGNAL(toggled(bool)), this, SLOT(autoSaveToPrompt(bool)));
	
	QRadioButton *rb2 = new QRadioButton(tr("Always save on run."));
   rb2->setToolTip(tr("If you try to run an unsaved file, it will automatically be saved."));
   rb2->setStyle(style);
   l2->addWidget(rb2);
   rb2->setChecked(Options::autoSaveOptions() == Options::ALWAYS);
   connect(rb2, SIGNAL(toggled(bool)), this, SLOT(autoSaveToAlways(bool)));
	
	QRadioButton *rb3 = new QRadioButton(tr("Do nothing on run."));
   rb3->setToolTip(tr("Nothing will happen if you try to run an unsaved file."));
   rb3->setStyle(style);
   l2->addWidget(rb3);
   rb3->setChecked(Options::autoSaveOptions() == Options::NEVER);
   connect(rb3, SIGNAL(toggled(bool)), this, SLOT(autoSaveToNever(bool)));

   group->setLayout(l2);
   mainLayout->addWidget(group);
   mainLayout->addSpacing(12);
   
   // ----------------------------------------------------------
   QPushButton *b = new QPushButton(QString("Change Font"));// (%1)").arg(m_font.toString()));
   b->setFont(m_font);
   m_fontButton = b;
   m_fontButton->setToolTip(tr("Change the font used for the main code editor."));

   m_fontButton->setFont(Options::getFont());
   connect(b, SIGNAL(clicked()), this, SLOT(changeFont()));

   mainLayout->addWidget(b);

   // ----------------------------------------------------------
/*   QHBoxLayout *lH = new QHBoxLayout;
   QLabel *fontLabel = new QLabel(tr("Family:"));
   m_fontCombo = new QFontComboBox;
   QLabel *sizeLabel = new QLabel(tr("Size:"));
   m_sizeCombo = new QComboBox;
   
   lH->addWidget(fontLabel);
   lH->addWidget(m_fontCombo);
   lH->addWidget(sizeLabel);
   lH->addWidget(m_sizeCombo);
   
   m_exampleText = new QLabel("Example Text");

   findSizes(m_fontCombo->currentFont());
   connect(m_fontCombo, SIGNAL(currentFontChanged(const QFont &)),
         this, SLOT(findSizes(const QFont &)));
   connect(m_fontCombo, SIGNAL(currentFontChanged(const QFont &)),
         this, SLOT(fontFamilyChanged(const QFont &)));
   connect(sizeCombo, SIGNAL(currentIndexChanged(const QString &)),
         this, SLOT(sizeChanged(const QString &)));
   connect(this, SIGNAL(fontChanged(const QFont &)), m_exampleText, */


   setLayout(mainLayout);
}

void EditingOptionsPage::syntaxPopupsChanged(int index) {
   Options::m_options->setSyntaxPopupsEnabled(index != Qt::Unchecked);
}

void EditingOptionsPage::errorHighlightingChanged(int index) {
   Options::m_options->setErrorHighlightingEnabled(index != Qt::Unchecked);
}

void EditingOptionsPage::autoIndentationChanged(int index) {
   Options::m_options->setAutoIndentationEnabled(index != Qt::Unchecked);
}

void EditingOptionsPage::tabWidthChanged(int index) {
   Options::m_options->setTabWidth(index);
}

void EditingOptionsPage::autoSaveToPrompt(bool doIfTrue) {
	if (doIfTrue) Options::m_options->setAutoSaveOptions(Options::PROMPT);
}

void EditingOptionsPage::autoSaveToAlways(bool doIfTrue) {
	if (doIfTrue) Options::m_options->setAutoSaveOptions(Options::ALWAYS);
}

void EditingOptionsPage::autoSaveToNever(bool doIfTrue) {
	if (doIfTrue) Options::m_options->setAutoSaveOptions(Options::NEVER);
}

void EditingOptionsPage::changeFont() {
   m_font = Options::getFont();
   m_font = QFontDialog::getFont(0, m_font);
   m_fontButton->setFont(m_font);
   m_fontButton->setText(QString("Change Font"));// (%1)").arg(m_font.toString()));

   Options::m_options->setFont(m_font);
   fontChanged(m_font);
}



DebuggingOptionsPage::DebuggingOptionsPage(QWidget *parent) : QWidget(parent)
{
   QVBoxLayout *mainLayout = new QVBoxLayout;
   QStyle *style = new QCleanlooksStyle();

   // ----------------------------------------------------------
   QGroupBox *group = new QGroupBox(tr("Debugging"));
   QVBoxLayout *l = new QVBoxLayout;
   QCheckBox *ch = new QCheckBox(tr("Disable editing during debugging (read-only)"));
   ch->setToolTip(tr("Enable/Disable editing-on-the-fly during debugging."));
   ch->setStyle(style);
   l->addWidget(ch);
   connect(ch, SIGNAL(stateChanged(int)), this, SLOT(readOnlyDebuggingChanged(int)));
   
   // TODO:  enable implicit rollback CheckBox iff not read-only
   ch = new QCheckBox(tr("Implicit rollback upon editing"));
   ch->setToolTip(tr("Enable/Disable implicit program rollback to earliest point "
                     "in execution, just before an edit would have been first "
                     "encountered. (currently must be enabled)"));
   ch->setStyle(style);
   
   // TODO:  Add support for turning on/off implicit rollback and reenable this Option
   ch->setEnabled(false);


   l->addWidget(ch);
   connect(ch, SIGNAL(stateChanged(int)), this, SLOT(implicitRollbackChanged(int)));

   QLabel *label  = new QLabel(tr("Number of previous instructions to depict:"));
   QSpinBox *tabs = new QSpinBox;
   tabs->setRange(0, 16);
   tabs->setValue(Options::noPreviousXInstructions());
   connect(tabs, SIGNAL(valueChanged(int)), Options::m_options, SLOT(setNoPreviousXInstructions(int)));

//   tabs->setSuffix(tr(" instructions"));
   tabs->setSpecialValueText(tr("Only show current PC"));
   const QString &toolTip = tr("Length of gradient, depicting the previous instructions executed during a pause in execution.");
   label->setToolTip(toolTip);
   tabs->setToolTip(toolTip);
   
   QHBoxLayout *lH = new QHBoxLayout;
   lH->addWidget(label);
   lH->addWidget(tabs);
   l->addLayout(lH);

   group->setLayout(l);
   mainLayout->addWidget(group);
   mainLayout->addSpacing(12);
   // ----------------------------------------------------------

   setLayout(mainLayout);
}

void DebuggingOptionsPage::readOnlyDebuggingChanged(int index) {
   Options::m_options->setReadOnlyDebuggingEnabled(index != Qt::Unchecked);
}

void DebuggingOptionsPage::implicitRollbackChanged(int index) {
   Options::m_options->setImplicitRollbackEnabled(index != Qt::Unchecked);
}

/*void DebuggingOptionsPage::fontFamilyChanged(const QFont &font) {
   m_font.setFamily(font.family());
   fontChanged(m_font);
}

void DebuggingOptionsPage::sizeChanged(const QString &str) {
   unsigned int fontSize = str.toInt();

   m_font.setPointSize(fontSize);
   fontChanged(m_font);
}

void DebuggingOptionsPage::findSizes(const QFont &font) {
    QFontDatabase fontDatabase;
    QString currentSize = m_sizeCombo->currentText();
    m_sizeCombo->blockSignals(true);
    m_sizeCombo->clear();

    int size;
    if(fontDatabase.isSmoothlyScalable(font.family(), fontDatabase.styleString(font))) {
        foreach(size, QFontDatabase::standardSizes()) {
            m_sizeCombo->addItem(QVariant(size).toString());
            m_sizeCombo->setEditable(true);
        }

    } else {
        foreach(size, fontDatabase.smoothSizes(font.family(), fontDatabase.styleString(font))) {
            m_sizeCombo->addItem(QVariant(size).toString());
            m_sizeCombo->setEditable(false);
        }
    }

    m_sizeCombo->blockSignals(false);

    int sizeIndex = m_sizeCombo->findText(currentSize);

    if(sizeIndex == -1)
        m_sizeCombo->setCurrentIndex(qMax(0, sizeCombo->count() / 3));
    else
        m_sizeCombo->setCurrentIndex(sizeIndex);
}*/






