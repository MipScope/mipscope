#include "Directives.H"
#include "../Identifier.H"

SingleIntegerDirectiveInitializer::~SingleIntegerDirectiveInitializer ()
{
	for (std::vector<Identifier*>::iterator it(m_values.begin()); it != m_values.end(); ++it) {
		delete *it;
	}
}

template<class Value> void SingleIntegerDirectiveInitializer::init (State* s, unsigned int address)
{
	Value*	buffer = new Value[m_values.size()];

	Value*	next = buffer;
	for (std::vector<Identifier*>::iterator it(m_values.begin()); it != m_values.end(); ++it) {
		*next++ = (*it)->getValue();
	}

	s->memcpy(address, buffer, m_values.size() * sizeof(Value));

	delete[] buffer;
}

void SingleIntegerDirectiveInitializer::init_byte (State* s, unsigned int address)
{
	init<unsigned char>(s, address);
}

void SingleIntegerDirectiveInitializer::init_half (State* s, unsigned int address)
{
	init<unsigned short>(s, address);
}

void SingleIntegerDirectiveInitializer::init_word (State* s, unsigned int address)
{
	init<unsigned int>(s, address);
}

void SingleIntegerDirectiveInitializer::get_referenced_labels (std::vector<AddressIdentifier*>& labels)
{
	for (std::vector<Identifier*>::iterator it(m_values.begin()); it != m_values.end(); ++it) {
		if (AddressIdentifier* ai = dynamic_cast<AddressIdentifier*>(*it)) {
			labels.push_back(ai);
		}
	}
}

RepeatIntegerDirectiveInitializer::~RepeatIntegerDirectiveInitializer ()
{
	delete m_value;
}

template<class Value> void RepeatIntegerDirectiveInitializer::init (State* s, unsigned int address)
{
	int	value = m_value->getValue();

	Value*	buffer = new Value[m_count];

	Value*	next = buffer;
	for (size_t i = 0; i < m_count; ++i) {
		*next++ = value;
	}

	s->memcpy(address, buffer, m_count * sizeof(Value));

	delete[] buffer;
}

void RepeatIntegerDirectiveInitializer::init_byte (State* s, unsigned int address)
{
	init<unsigned char>(s, address);
}

void RepeatIntegerDirectiveInitializer::init_half (State* s, unsigned int address)
{
	init<unsigned short>(s, address);
}

void RepeatIntegerDirectiveInitializer::init_word (State* s, unsigned int address)
{
	init<unsigned int>(s, address);
}

void RepeatIntegerDirectiveInitializer::get_referenced_labels (std::vector<AddressIdentifier*>& labels)
{
	if (AddressIdentifier* ai = dynamic_cast<AddressIdentifier*>(m_value)) {
		labels.push_back(ai);
	}
}

