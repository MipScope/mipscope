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
   file: Options.C
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 5/31/2007
\* ---------------------------------------------- */

#include "Options.H"
#include "TextEditor.H"
#include "Gui.H"
#include "EditorPane.H"
#include "RegisterView.H"
#include "StatementListing.H"
#include <QtGui>

Options   *Options::m_options  = NULL;
QSettings *Options::m_settings = NULL;

Options::Options(Gui *gui) : m_gui(gui)
{
   m_options  = this;
   m_settings = new QSettings("Brown University", PROJECT_NAME);

   resetDefaults();
}

Options::~Options() { }

void Options::resetDefaults() {
   // General
   if (!VERBOSE)
      m_verbosity = m_settings->value("verbosity", false).toBool();
   else m_verbosity = VERBOSE;
   VERBOSE = m_verbosity;
   m_registerAliases = m_settings->value("registerAliases", true).toBool();
   m_displayBase = m_settings->value("displayBase", 16).toInt();

   m_showAllInstructions = m_settings->value("showAllInstructions", true).toBool();
   m_templatePath = m_settings->value("templatePath", QString(TEMPLATE)).toString();
   m_xspimPath = m_settings->value("xspimPath", QString("/course/cs031/pro/spim/xspim")).toString();

   // Editing
   m_syntaxPopupsEnabled = m_settings->value("syntaxPopupsEnabled", true).toBool();
   m_errorHighlightingEnabled = m_settings->value("errorHighlightingEnabled", true).toBool();
   m_autoIndentationEnabled   = m_settings->value("autoIndentationEnabled", true).toBool();
   m_tabWidth = m_settings->value("tabWidth", 4).toInt();
   m_font = QFont();
   m_font.fromString(m_settings->value("font", QFont("Courier", 11).toString()).toString());

	//Auto-Save	
	int temp = m_settings->value("autoSaveOptions", 4).toInt();
	
	switch (temp) {
		case 1:
			m_autoSaveOptions = ALWAYS;
			break;
		case 2:
			m_autoSaveOptions = NEVER;
			break;
		default:
			m_autoSaveOptions = PROMPT;
			break;
	}
	
   // Debugging
   m_readOnlyDebuggingEnabled = m_settings->value("readOnlyDebuggingEnabled", false).toBool();
   m_implicitRollbackEnabled  = m_settings->value("implicitRollbackEnabled", true).toBool();
   m_noPreviousXInstructions  = m_settings->value("noPreviousXInstructions", 2).toInt();
}

void Options::setupConnections(EditorPane *editorPane) {
   connect(this, SIGNAL(fontChanged(const QFont&)), editorPane, SLOT(currentFontChanged(const QFont&)));
   connect(this, SIGNAL(registerAliasesChanged(bool)), m_gui->getRegisterView(), SLOT(updateDisplay(bool)));
   connect(this, SIGNAL(displayBaseChanged(int)), m_gui->getRegisterView(), SLOT(updateBase(int)));
   connect(this, SIGNAL(showAllInstructionsChanged(bool)), m_gui->getStatementListing(), SLOT(showAllInstructionsChanged(bool)));
}

// -----------------------------------------------
//                   General
// -----------------------------------------------
bool Options::verbosity() {
   return m_options->m_verbosity;
}

bool Options::registerAliases() {
   return m_options->m_registerAliases;
}

int Options::dislayBase() {
//   cerr << "displayBase: " << m_options->m_displayBase << endl;
   return m_options->m_displayBase;
}

bool Options::showAllInstructions() {
   return m_options->m_showAllInstructions;
}

QString Options::getTemplatePath() {
   return m_options->m_templatePath;
}

QString Options::getXSpimPath() {
   return m_options->m_xspimPath;
}


void Options::setVerbosity(bool v) {
   m_options->m_verbosity = v;
   VERBOSE = v;
   m_settings->setValue("verbosity", v);
   m_options->verbosityChanged(v);
}

void Options::setRegisterAliases(bool a) {
   m_options->m_registerAliases = a;
   m_settings->setValue("registerAliases", a);
   m_options->registerAliasesChanged(a);
}

void Options::setDislayBase(unsigned int d) {
   m_options->m_displayBase = d;
   m_settings->setValue("displayBase", d);
   m_options->displayBaseChanged(d);
}

void Options::setShowAllInstructions(bool s) {
   cerr << "Options::setShowAllInstructions: " << s << endl;

   m_options->m_showAllInstructions = s;
   m_settings->setValue("showAllInstructions", s);
   m_options->showAllInstructionsChanged(s);
}

void Options::setTemplatePath(const QString &s) {
   m_options->m_templatePath = s;
   m_settings->setValue("templatePath", s);
   m_options->templatePathChanged(s);
}

void Options::setXSpimPath(const QString &s) {
   m_options->m_xspimPath = s;
   m_settings->setValue("xspimPath", s);
   m_options->xspimPathChanged(s);
}


// -----------------------------------------------
//                   Editing
// -----------------------------------------------
bool Options::syntaxPopupsEnabled() {
   return m_options->m_syntaxPopupsEnabled;
}

bool Options::errorHighlightingEnabled() {
   return m_options->m_errorHighlightingEnabled;
}

bool Options::autoIndentationEnabled() {
   return m_options->m_autoIndentationEnabled;
}

unsigned int Options::tabWidth() {
   return m_options->m_tabWidth;
}

QFont Options::getFont() {
   return m_options->m_font;
}

void Options::setSyntaxPopupsEnabled(bool b) {
   m_options->m_syntaxPopupsEnabled = b;
   m_settings->setValue("syntaxPopupsEnabled", b);
   m_options->syntaxPopupsEnabledChanged(b);
}

void Options::setErrorHighlightingEnabled(bool b) {
   m_options->m_errorHighlightingEnabled = b;
   m_settings->setValue("errorHighlightingEnabled", b);
   m_options->errorHighlightingEnabledChanged(b);
}

void Options::setAutoIndentationEnabled(bool b) {
   m_options->m_autoIndentationEnabled = b;
   m_settings->setValue("autoIndentationEnabled", b);
   m_options->autoIndentationEnabledChanged(b);
}

void Options::setTabWidth(unsigned int w) {
   m_options->m_tabWidth = w;
   m_settings->setValue("tabWidth", w);
   m_options->tabWidthChanged(w);
}

void Options::setFont(const QFont &f) {
   m_options->m_font = f;
   m_settings->setValue("font", f.toString());
   m_options->fontChanged(f);
}

// -----------------------------------------------
//                   Auto-Save
// -----------------------------------------------

Options::AutoSaveOptions Options::autoSaveOptions() {
	return m_options->m_autoSaveOptions;
}

void Options::setAutoSaveOptions(Options::AutoSaveOptions aso) {
	
	m_options->m_autoSaveOptions = aso;
	m_settings->setValue("autoSaveOptions", aso);
   m_options->autoSaveOptionsChanged(aso);
}

// -----------------------------------------------
//                   Debugging
// -----------------------------------------------
bool Options::readOnlyDebuggingEnabled() {
   return m_options->m_readOnlyDebuggingEnabled;
}

bool Options::implicitRollbackEnabled() {
   return m_options->m_implicitRollbackEnabled;
}

unsigned int Options::noPreviousXInstructions() {
   return m_options->m_noPreviousXInstructions;
}

void Options::setReadOnlyDebuggingEnabled(bool b) {
   m_options->m_readOnlyDebuggingEnabled = b;
   m_settings->setValue("readOnlyDebuggingEnabled", b);
   m_options->readOnlyDebuggingEnabledChanged(b);
}

void Options::setImplicitRollbackEnabled(bool b) {
   m_options->m_implicitRollbackEnabled = b;
   m_settings->setValue("implicitRollbackEnabled", b);
   m_options->implicitRollbackEnabledChanged(b);
}

void Options::setNoPreviousXInstructions(int no) {
   m_options->m_noPreviousXInstructions = no;
   m_settings->setValue("noPreviousXInstructions", no);
   m_options->noPreviousXInstructionsChanged(no);
}

