/*
www.sourceforge.net/projects/tinyxml
Original code (2.0 and earlier )copyright (c) 2000-2006 Lee Thomason (www.grinninglizard.com)

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any
damages arising from the use of this software.

Permission is granted to anyone to use this software for any
purpose, including commercial applications, and to alter it and
redistribute it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must
not claim that you wrote the original software. If you use this
software in a product, an acknowledgment in the product documentation
would be appreciated but is not required.

2. Altered source versions must be plainly marked as such, and
must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any source
distribution.
*/

#include "hpl1/engine/impl/tinyXML/tinyxml.h"
#include "common/file.h"
#include "common/debug.h"
#include "hpl1/detection.h"

bool TiXmlBase::condenseWhiteSpace = true;

void TiXmlBase::PutString(const TIXML_STRING &str, TIXML_STRING *outString) {
	int i = 0;

	while (i < (int)str.size()) {
		unsigned char c = (unsigned char)str[i];

		if (c == '&' && i < ((int)str.size() - 2) && str[i + 1] == '#' && str[i + 2] == 'x') {
			// Hexadecimal character reference.
			// Pass through unchanged.
			// &#xA9;	-- copyright symbol, for example.
			//
			// The -1 is a bug fix from Rob Laveaux. It keeps
			// an overflow from happening if there is no ';'.
			// There are actually 2 ways to exit this loop -
			// while fails (error case) and break (semicolon found).
			// However, there is no mechanism (currently) for
			// this function to return an error.
			while (i < (int)str.size() - 1) {
				(*outString) += Common::String(str.c_str() + i, 1);
				++i;
				if (str[i] == ';')
					break;
			}
		} else if (c == '&') {
			(*outString) += Common::String(entity[0].str, entity[0].strLength);
			++i;
		} else if (c == '<') {
			(*outString) += Common::String(entity[1].str, entity[1].strLength);
			++i;
		} else if (c == '>') {
			(*outString) += Common::String(entity[2].str, entity[2].strLength);
			++i;
		} else if (c == '\"') {
			(*outString) += Common::String(entity[3].str, entity[3].strLength);
			++i;
		} else if (c == '\'') {
			(*outString) += Common::String(entity[4].str, entity[4].strLength);
			++i;
		} else if (c < 32) {
			// Easy pass at non-alpha/numeric/symbol
			// Below 32 is symbolic.
			(*outString) += Common::String::format("&#x%02X;", (unsigned)(c & 0xff));
			++i;
		} else {
			(*outString) += (char)c;
			++i;
		}
	}
}

TiXmlNode::TiXmlNode(NodeType _type) : TiXmlBase() {
	parent = 0;
	type = _type;
	firstChild = 0;
	lastChild = 0;
	prev = 0;
	next = 0;
}

TiXmlNode::~TiXmlNode() {
	TiXmlNode *node = firstChild;
	TiXmlNode *temp = 0;

	while (node) {
		temp = node;
		node = node->next;
		delete temp;
	}
}

void TiXmlNode::CopyTo(TiXmlNode *target) const {
	target->SetValue(value.c_str());
	target->userData = userData;
}

void TiXmlNode::Clear() {
	TiXmlNode *node = firstChild;
	TiXmlNode *temp = 0;

	while (node) {
		temp = node;
		node = node->next;
		delete temp;
	}

	firstChild = 0;
	lastChild = 0;
}

TiXmlNode *TiXmlNode::LinkEndChild(TiXmlNode *node) {
	assert(node->parent == 0 || node->parent == this);
	assert(node->GetDocument() == 0 || node->GetDocument() == this->GetDocument());

	if (node->Type() == TiXmlNode::DOCUMENT) {
		delete node;
		if (GetDocument())
			GetDocument()->SetError(TIXML_ERROR_DOCUMENT_TOP_ONLY, 0, 0, TIXML_ENCODING_UNKNOWN);
		return 0;
	}

	node->parent = this;

	node->prev = lastChild;
	node->next = 0;

	if (lastChild)
		lastChild->next = node;
	else
		firstChild = node; // it was an empty list.

	lastChild = node;
	return node;
}

TiXmlNode *TiXmlNode::InsertEndChild(const TiXmlNode &addThis) {
	if (addThis.Type() == TiXmlNode::DOCUMENT) {
		if (GetDocument())
			GetDocument()->SetError(TIXML_ERROR_DOCUMENT_TOP_ONLY, 0, 0, TIXML_ENCODING_UNKNOWN);
		return 0;
	}
	TiXmlNode *node = addThis.Clone();
	if (!node)
		return 0;

	return LinkEndChild(node);
}

TiXmlNode *TiXmlNode::InsertBeforeChild(TiXmlNode *beforeThis, const TiXmlNode &addThis) {
	if (!beforeThis || beforeThis->parent != this) {
		return 0;
	}
	if (addThis.Type() == TiXmlNode::DOCUMENT) {
		if (GetDocument())
			GetDocument()->SetError(TIXML_ERROR_DOCUMENT_TOP_ONLY, 0, 0, TIXML_ENCODING_UNKNOWN);
		return 0;
	}

	TiXmlNode *node = addThis.Clone();
	if (!node)
		return 0;
	node->parent = this;

	node->next = beforeThis;
	node->prev = beforeThis->prev;
	if (beforeThis->prev) {
		beforeThis->prev->next = node;
	} else {
		assert(firstChild == beforeThis);
		firstChild = node;
	}
	beforeThis->prev = node;
	return node;
}

TiXmlNode *TiXmlNode::InsertAfterChild(TiXmlNode *afterThis, const TiXmlNode &addThis) {
	if (!afterThis || afterThis->parent != this) {
		return 0;
	}
	if (addThis.Type() == TiXmlNode::DOCUMENT) {
		if (GetDocument())
			GetDocument()->SetError(TIXML_ERROR_DOCUMENT_TOP_ONLY, 0, 0, TIXML_ENCODING_UNKNOWN);
		return 0;
	}

	TiXmlNode *node = addThis.Clone();
	if (!node)
		return 0;
	node->parent = this;

	node->prev = afterThis;
	node->next = afterThis->next;
	if (afterThis->next) {
		afterThis->next->prev = node;
	} else {
		assert(lastChild == afterThis);
		lastChild = node;
	}
	afterThis->next = node;
	return node;
}

TiXmlNode *TiXmlNode::ReplaceChild(TiXmlNode *replaceThis, const TiXmlNode &withThis) {
	if (replaceThis->parent != this)
		return 0;

	TiXmlNode *node = withThis.Clone();
	if (!node)
		return 0;

	node->next = replaceThis->next;
	node->prev = replaceThis->prev;

	if (replaceThis->next)
		replaceThis->next->prev = node;
	else
		lastChild = node;

	if (replaceThis->prev)
		replaceThis->prev->next = node;
	else
		firstChild = node;

	delete replaceThis;
	node->parent = this;
	return node;
}

bool TiXmlNode::RemoveChild(TiXmlNode *removeThis) {
	if (removeThis->parent != this) {
		assert(0);
		return false;
	}

	if (removeThis->next)
		removeThis->next->prev = removeThis->prev;
	else
		lastChild = removeThis->prev;

	if (removeThis->prev)
		removeThis->prev->next = removeThis->next;
	else
		firstChild = removeThis->next;

	delete removeThis;
	return true;
}

const TiXmlNode *TiXmlNode::FirstChild(const char *_value) const {
	const TiXmlNode *node;
	for (node = firstChild; node; node = node->next) {
		if (strcmp(node->Value(), _value) == 0)
			return node;
	}
	return 0;
}

const TiXmlNode *TiXmlNode::LastChild(const char *_value) const {
	const TiXmlNode *node;
	for (node = lastChild; node; node = node->prev) {
		if (strcmp(node->Value(), _value) == 0)
			return node;
	}
	return 0;
}

const TiXmlNode *TiXmlNode::IterateChildren(const TiXmlNode *previous) const {
	if (!previous) {
		return FirstChild();
	} else {
		assert(previous->parent == this);
		return previous->NextSibling();
	}
}

const TiXmlNode *TiXmlNode::IterateChildren(const char *val, const TiXmlNode *previous) const {
	if (!previous) {
		return FirstChild(val);
	} else {
		assert(previous->parent == this);
		return previous->NextSibling(val);
	}
}

const TiXmlNode *TiXmlNode::NextSibling(const char *_value) const {
	const TiXmlNode *node;
	for (node = next; node; node = node->next) {
		if (strcmp(node->Value(), _value) == 0)
			return node;
	}
	return 0;
}

const TiXmlNode *TiXmlNode::PreviousSibling(const char *_value) const {
	const TiXmlNode *node;
	for (node = prev; node; node = node->prev) {
		if (strcmp(node->Value(), _value) == 0)
			return node;
	}
	return 0;
}

void TiXmlElement::RemoveAttribute(const char *name) {
#ifdef TIXML_USE_STL
	TIXML_STRING str(name);
	TiXmlAttribute *node = attributeSet.Find(str);
#else
	TiXmlAttribute *node = attributeSet.Find(name);
#endif
	if (node) {
		attributeSet.Remove(node);
		delete node;
	}
}

const TiXmlElement *TiXmlNode::FirstChildElement() const {
	const TiXmlNode *node;

	for (node = FirstChild();
		 node;
		 node = node->NextSibling()) {
		if (node->ToElement())
			return node->ToElement();
	}
	return 0;
}

const TiXmlElement *TiXmlNode::FirstChildElement(const char *_value) const {
	const TiXmlNode *node;

	for (node = FirstChild(_value);
		 node;
		 node = node->NextSibling(_value)) {
		if (node->ToElement())
			return node->ToElement();
	}
	return 0;
}

const TiXmlElement *TiXmlNode::NextSiblingElement() const {
	const TiXmlNode *node;

	for (node = NextSibling();
		 node;
		 node = node->NextSibling()) {
		if (node->ToElement())
			return node->ToElement();
	}
	return 0;
}

const TiXmlElement *TiXmlNode::NextSiblingElement(const char *_value) const {
	const TiXmlNode *node;

	for (node = NextSibling(_value);
		 node;
		 node = node->NextSibling(_value)) {
		if (node->ToElement())
			return node->ToElement();
	}
	return 0;
}

const TiXmlDocument *TiXmlNode::GetDocument() const {
	const TiXmlNode *node;

	for (node = this; node; node = node->parent) {
		if (node->ToDocument())
			return node->ToDocument();
	}
	return 0;
}

TiXmlElement::TiXmlElement(const char *_value)
	: TiXmlNode(TiXmlNode::ELEMENT) {
	firstChild = lastChild = 0;
	value = _value;
}

#ifdef TIXML_USE_STL
TiXmlElement::TiXmlElement(const std::string &_value)
	: TiXmlNode(TiXmlNode::ELEMENT) {
	firstChild = lastChild = 0;
	value = _value;
}
#endif

TiXmlElement::TiXmlElement(const TiXmlElement &copy)
	: TiXmlNode(TiXmlNode::ELEMENT) {
	firstChild = lastChild = 0;
	copy.CopyTo(this);
}

void TiXmlElement::operator=(const TiXmlElement &base) {
	ClearThis();
	base.CopyTo(this);
}

TiXmlElement::~TiXmlElement() {
	ClearThis();
}

void TiXmlElement::ClearThis() {
	Clear();
	while (attributeSet.First()) {
		TiXmlAttribute *node = attributeSet.First();
		attributeSet.Remove(node);
		delete node;
	}
}

const char *TiXmlElement::Attribute(const char *name) const {
	const TiXmlAttribute *node = attributeSet.Find(name);
	if (node)
		return node->Value();
	return 0;
}

#ifdef TIXML_USE_STL
const std::string *TiXmlElement::Attribute(const std::string &name) const {
	const TiXmlAttribute *node = attributeSet.Find(name);
	if (node)
		return &node->ValueStr();
	return 0;
}
#endif

const char *TiXmlElement::Attribute(const char *name, int *i) const {
	const char *s = Attribute(name);
	if (i) {
		if (s) {
			*i = atoi(s);
		} else {
			*i = 0;
		}
	}
	return s;
}

#ifdef TIXML_USE_STL
const std::string *TiXmlElement::Attribute(const std::string &name, int *i) const {
	const std::string *s = Attribute(name);
	if (i) {
		if (s) {
			*i = atoi(s->c_str());
		} else {
			*i = 0;
		}
	}
	return s;
}
#endif

const char *TiXmlElement::Attribute(const char *name, double *d) const {
	const char *s = Attribute(name);
	if (d) {
		if (s) {
			*d = atof(s);
		} else {
			*d = 0;
		}
	}
	return s;
}

#ifdef TIXML_USE_STL
const std::string *TiXmlElement::Attribute(const std::string &name, double *d) const {
	const std::string *s = Attribute(name);
	if (d) {
		if (s) {
			*d = atof(s->c_str());
		} else {
			*d = 0;
		}
	}
	return s;
}
#endif

int TiXmlElement::QueryIntAttribute(const char *name, int *ival) const {
	const TiXmlAttribute *node = attributeSet.Find(name);
	if (!node)
		return TIXML_NO_ATTRIBUTE;
	return node->QueryIntValue(ival);
}

#ifdef TIXML_USE_STL
int TiXmlElement::QueryIntAttribute(const std::string &name, int *ival) const {
	const TiXmlAttribute *node = attributeSet.Find(name);
	if (!node)
		return TIXML_NO_ATTRIBUTE;
	return node->QueryIntValue(ival);
}
#endif

int TiXmlElement::QueryDoubleAttribute(const char *name, double *dval) const {
	const TiXmlAttribute *node = attributeSet.Find(name);
	if (!node)
		return TIXML_NO_ATTRIBUTE;
	return node->QueryDoubleValue(dval);
}

#ifdef TIXML_USE_STL
int TiXmlElement::QueryDoubleAttribute(const std::string &name, double *dval) const {
	const TiXmlAttribute *node = attributeSet.Find(name);
	if (!node)
		return TIXML_NO_ATTRIBUTE;
	return node->QueryDoubleValue(dval);
}
#endif

void TiXmlElement::SetAttribute(const char *name, int val) {
	Common::String sval = Common::String::format("%d", val); 
	SetAttribute(name, sval.c_str());
}

#ifdef TIXML_USE_STL
void TiXmlElement::SetAttribute(const std::string &name, int val) {
	std::ostringstream oss;
	oss << val;
	SetAttribute(name, oss.str());
}
#endif

void TiXmlElement::SetDoubleAttribute(const char *name, double val) {
	Common::String sval = Common::String::format("%f", val); 
	SetAttribute(name, sval.c_str());
}

void TiXmlElement::SetAttribute(const char *cname, const char *cvalue) {
#ifdef TIXML_USE_STL
	TIXML_STRING _name(cname);
	TIXML_STRING _value(cvalue);
#else
	const char *_name = cname;
	const char *_value = cvalue;
#endif

	TiXmlAttribute *node = attributeSet.Find(_name);
	if (node) {
		node->SetValue(_value);
		return;
	}

	TiXmlAttribute *attrib = new TiXmlAttribute(cname, cvalue);
	if (attrib) {
		attributeSet.Add(attrib);
	} else {
		TiXmlDocument *document = GetDocument();
		if (document)
			document->SetError(TIXML_ERROR_OUT_OF_MEMORY, 0, 0, TIXML_ENCODING_UNKNOWN);
	}
}

#ifdef TIXML_USE_STL
void TiXmlElement::SetAttribute(const std::string &name, const std::string &_value) {
	TiXmlAttribute *node = attributeSet.Find(name);
	if (node) {
		node->SetValue(_value);
		return;
	}

	TiXmlAttribute *attrib = new TiXmlAttribute(name, _value);
	if (attrib) {
		attributeSet.Add(attrib);
	} else {
		TiXmlDocument *document = GetDocument();
		if (document)
			document->SetError(TIXML_ERROR_OUT_OF_MEMORY, 0, 0, TIXML_ENCODING_UNKNOWN);
	}
}
#endif

void TiXmlElement::Print(Common::DumpFile &file, int depth) const {
	int i;
	for (i = 0; i < depth; i++) {
		file.writeString("    ");
	}

	file.writeString("<" + value);

	const TiXmlAttribute *attrib;
	for (attrib = attributeSet.First(); attrib; attrib = attrib->Next()) {
		file.writeString(" "); 
		attrib->Print(file, depth);
	}

	// There are 3 different formatting approaches:
	// 1) An element without children is printed as a <foo /> node
	// 2) An element with only a text child is printed as <foo> text </foo>
	// 3) An element with children is printed on multiple lines.
	TiXmlNode *node;
	if (!firstChild) {
		file.writeString(" />");
	} else if (firstChild == lastChild && firstChild->ToText()) {
		file.writeString(">");
		firstChild->Print(file, depth + 1);
		file.writeString("</" + value + ">");
	} else {
		file.writeString(">");

		for (node = firstChild; node; node = node->NextSibling()) {
			if (!node->ToText()) {
				file.writeString("\n");
			}
			node->Print(file, depth + 1);
		}
		file.writeString("\n");
		for (i = 0; i < depth; ++i) {
			file.writeString("    ");
		}
		file.writeString("</" + value + ">");
	}
}

void TiXmlElement::CopyTo(TiXmlElement *target) const {
	// superclass:
	TiXmlNode::CopyTo(target);

	// Element class:
	// Clone the attributes, then clone the children.
	const TiXmlAttribute *attribute = 0;
	for (attribute = attributeSet.First();
		 attribute;
		 attribute = attribute->Next()) {
		target->SetAttribute(attribute->Name(), attribute->Value());
	}

	TiXmlNode *node = 0;
	for (node = firstChild; node; node = node->NextSibling()) {
		target->LinkEndChild(node->Clone());
	}
}

bool TiXmlElement::Accept(TiXmlVisitor *visitor) const {
	if (visitor->VisitEnter(*this, attributeSet.First())) {
		for (const TiXmlNode *node = FirstChild(); node; node = node->NextSibling()) {
			if (!node->Accept(visitor))
				break;
		}
	}
	return visitor->VisitExit(*this);
}

TiXmlNode *TiXmlElement::Clone() const {
	TiXmlElement *clone = new TiXmlElement(Value());
	if (!clone)
		return 0;

	CopyTo(clone);
	return clone;
}

const char *TiXmlElement::GetText() const {
	const TiXmlNode *child = this->FirstChild();
	if (child) {
		const TiXmlText *childText = child->ToText();
		if (childText) {
			return childText->Value();
		}
	}
	return 0;
}

TiXmlDocument::TiXmlDocument() : TiXmlNode(TiXmlNode::DOCUMENT) {
	tabsize = 4;
	useMicrosoftBOM = false;
	ClearError();
}

TiXmlDocument::TiXmlDocument(const char *documentName) : TiXmlNode(TiXmlNode::DOCUMENT) {
	tabsize = 4;
	useMicrosoftBOM = false;
	value = documentName;
	ClearError();
}

#ifdef TIXML_USE_STL
TiXmlDocument::TiXmlDocument(const std::string &documentName) : TiXmlNode(TiXmlNode::DOCUMENT) {
	tabsize = 4;
	useMicrosoftBOM = false;
	value = documentName;
	ClearError();
}
#endif

TiXmlDocument::TiXmlDocument(const TiXmlDocument &copy) : TiXmlNode(TiXmlNode::DOCUMENT) {
	copy.CopyTo(this);
}

void TiXmlDocument::operator=(const TiXmlDocument &copy) {
	Clear();
	copy.CopyTo(this);
}

bool TiXmlDocument::LoadFile(TiXmlEncoding encoding) {
	// See STL_STRING_BUG below.
	// StringToBuffer buf( value );

	return LoadFile(Value(), encoding);
}

bool TiXmlDocument::SaveFile() const {
	// See STL_STRING_BUG below.
	//	StringToBuffer buf( value );
	//
	//	if ( buf.buffer && SaveFile( buf.buffer ) )
	//		return true;
	//
	//	return false;
	return SaveFile(Value());
}

bool TiXmlDocument::LoadFile(const char *filename, TiXmlEncoding encoding) {
	// There was a really terrifying little bug here. The code:
	//		value = filename
	// in the STL case, cause the assignment method of the std::string to
	// be called. What is strange, is that the std::string had the same
	// address as it's c_str() method, and so bad things happen. Looks
	// like a bug in the Microsoft STL implementation.
	// Add an extra string to avoid the crash.
	value = filename;

	// reading in binary mode so that tinyxml can normalize the EOL
	Common::File file; 
	file.open(value); 
	
	if (file.isOpen()) {
		bool result = LoadFile(file, encoding);
		return result;
	} else {
		debugC(Hpl1::kDebugFilePath, "file %s not found", value.c_str()); 
		SetError(TIXML_ERROR_OPENING_FILE, 0, 0, TIXML_ENCODING_UNKNOWN);
		return false;
	}
}

bool TiXmlDocument::LoadFile(Common::File &file, TiXmlEncoding encoding) {
	if (file.err()) {
		debugC(Hpl1::kDebugResourceLoading, "file %s could not be read", file.getName()); 
		SetError(TIXML_ERROR_OPENING_FILE, 0, 0, TIXML_ENCODING_UNKNOWN);
		return false;
	}

	// Delete the existing data:
	Clear();
	location.Clear();

	// Get the file size, so we can pre-allocate the string. HUGE speed impact.
	long length = file.size();

	// Strange case, but good to handle up front.
	if (length == 0) {
		SetError(TIXML_ERROR_DOCUMENT_EMPTY, 0, 0, TIXML_ENCODING_UNKNOWN);
		return false;
	}

	// If we have a file, assume it is all one big XML file, and read it in.
	// The document parser may decide the document ends sooner than the entire file, however.
	TIXML_STRING data;

	// Subtle bug here. TinyXml did use fgets. But from the XML spec:
	// 2.11 End-of-Line Handling
	// <snip>
	// <quote>
	// ...the XML processor MUST behave as if it normalized all line breaks in external
	// parsed entities (including the document entity) on input, before parsing, by translating
	// both the two-character sequence #xD #xA and any #xD that is not followed by #xA to
	// a single #xA character.
	// </quote>
	//
	// It is not clear fgets does that, and certainly isn't clear it works cross platform.
	// Generally, you expect fgets to translate from the convention of the OS to the c/unix
	// convention, and not work generally.

	/*
	while( fgets( buf, sizeof(buf), file ) )
	{
		data += buf;
	}
	*/

	char *buf = new char[length + 1];
	buf[0] = 0;

	file.read(buf, length); 
	if (file.err()) {
		debugC(Hpl1::kDebugResourceLoading, "file read for %s failed", file.getName()); 
		delete[] buf;
		SetError(TIXML_ERROR_OPENING_FILE, 0, 0, TIXML_ENCODING_UNKNOWN);
		return false;
	}

	const char *lastPos = buf;
	const char *p = buf;

	buf[length] = 0;
	while (*p) {
		assert(p < (buf + length));
		if (*p == 0xa) {
			// Newline character. No special rules for this. Append all the characters
			// since the last string, and include the newline.
			data += Common::String(lastPos, (p - lastPos + 1)); // append, include the newline
			++p;                                     // move past the newline
			lastPos = p;                             // and point to the new buffer (may be 0)
			assert(p <= (buf + length));
		} else if (*p == 0xd) {
			// Carriage return. Append what we have so far, then
			// handle moving forward in the buffer.
			if ((p - lastPos) > 0) {
				data += Common::String(lastPos, p - lastPos); // do not add the CR
			}
			data += (char)0xa; // a proper newline

			if (*(p + 1) == 0xa) {
				// Carriage return - new line sequence
				p += 2;
				lastPos = p;
				assert(p <= (buf + length));
			} else {
				// it was followed by something else...that is presumably characters again.
				++p;
				lastPos = p;
				assert(p <= (buf + length));
			}
		} else {
			++p;
		}
	}
	// Handle any left over characters.
	if (p - lastPos) {
		data += Common::String(lastPos, p - lastPos);
	}
	delete[] buf;
	buf = 0;

	Parse(data.c_str(), 0, encoding);

	if (Error())
		return false;
	else
		return true;
}

bool TiXmlDocument::SaveFile(const char *filename) const {
	// The old c stuff lives on...
	Common::DumpFile df;
	df.open(filename); 
	if (df.isOpen()) {
		bool result = SaveFile(df);
		return result;
	}
	debugC(Hpl1::kDebugFilePath, "file %s not found", value.c_str());
	return false;
}

bool TiXmlDocument::SaveFile(Common::DumpFile &fp) const {
	if (useMicrosoftBOM) {
		const unsigned char TIXML_UTF_LEAD_0 = 0xefU;
		const unsigned char TIXML_UTF_LEAD_1 = 0xbbU;
		const unsigned char TIXML_UTF_LEAD_2 = 0xbfU;
		fp.writeByte(TIXML_UTF_LEAD_0); 
		fp.writeByte(TIXML_UTF_LEAD_1); 
		fp.writeByte(TIXML_UTF_LEAD_2); 
	}
	Print(fp, 0);
	return fp.err();
}

void TiXmlDocument::CopyTo(TiXmlDocument *target) const {
	TiXmlNode::CopyTo(target);

	target->error = error;
	target->errorDesc = errorDesc.c_str();

	TiXmlNode *node = 0;
	for (node = firstChild; node; node = node->NextSibling()) {
		target->LinkEndChild(node->Clone());
	}
}

TiXmlNode *TiXmlDocument::Clone() const {
	TiXmlDocument *clone = new TiXmlDocument();
	if (!clone)
		return 0;

	CopyTo(clone);
	return clone;
}

void TiXmlDocument::Print(Common::DumpFile &cfile, int depth) const {
	for (const TiXmlNode *node = FirstChild(); node; node = node->NextSibling()) {
		node->Print(cfile, depth);
		cfile.writeString("\n"); 
	}
}

bool TiXmlDocument::Accept(TiXmlVisitor *visitor) const {
	if (visitor->VisitEnter(*this)) {
		for (const TiXmlNode *node = FirstChild(); node; node = node->NextSibling()) {
			if (!node->Accept(visitor))
				break;
		}
	}
	return visitor->VisitExit(*this);
}

const TiXmlAttribute *TiXmlAttribute::Next() const {
	// We are using knowledge of the sentinel. The sentinel
	// have a value or name.
	if (next->value.empty() && next->name.empty())
		return 0;
	return next;
}

/*
TiXmlAttribute* TiXmlAttribute::Next()
{
	// We are using knowledge of the sentinel. The sentinel
	// have a value or name.
	if ( next->value.empty() && next->name.empty() )
		return 0;
	return next;
}
*/

const TiXmlAttribute *TiXmlAttribute::Previous() const {
	// We are using knowledge of the sentinel. The sentinel
	// have a value or name.
	if (prev->value.empty() && prev->name.empty())
		return 0;
	return prev;
}

/*
TiXmlAttribute* TiXmlAttribute::Previous()
{
	// We are using knowledge of the sentinel. The sentinel
	// have a value or name.
	if ( prev->value.empty() && prev->name.empty() )
		return 0;
	return prev;
}
*/

void TiXmlAttribute::Print(Common::DumpFile *cfile, int /*depth*/, TIXML_STRING *str) const {
	TIXML_STRING n, v;

	PutString(name, &n);
	PutString(value, &v);

	if (value.find('\"') == TIXML_STRING::npos) {
		if (cfile) {
			cfile->writeString(n + "=\"" + v + "\"");
		}
		if (str) {
			(*str) += n;
			(*str) += "=\"";
			(*str) += v;
			(*str) += "\"";
		}
	} else {
		if (cfile) {
			cfile->writeString(n + "='" + v + "'");
		}
		if (str) {
			(*str) += n;
			(*str) += "='";
			(*str) += v;
			(*str) += "'";
		}
	}
}

int TiXmlAttribute::QueryIntValue(int *ival) const {
	if (sscanf(value.c_str(), "%d", ival) == 1)
		return TIXML_SUCCESS;
	return TIXML_WRONG_TYPE;
}

int TiXmlAttribute::QueryDoubleValue(double *dval) const {
	if (sscanf(value.c_str(), "%lf", dval) == 1)
		return TIXML_SUCCESS;
	return TIXML_WRONG_TYPE;
}

void TiXmlAttribute::SetIntValue(int _value) {
	Common::String sval = Common::String::format("%d", _value); 
	SetValue(sval.c_str());
}

void TiXmlAttribute::SetDoubleValue(double _value) {
	Common::String sval = Common::String::format("%f", _value); 
	SetValue(sval.c_str());
}

int TiXmlAttribute::IntValue() const {
	return atoi(value.c_str());
}

double TiXmlAttribute::DoubleValue() const {
	return atof(value.c_str());
}

TiXmlComment::TiXmlComment(const TiXmlComment &copy) : TiXmlNode(TiXmlNode::COMMENT) {
	copy.CopyTo(this);
}

void TiXmlComment::operator=(const TiXmlComment &base) {
	Clear();
	base.CopyTo(this);
}

void TiXmlComment::Print(Common::DumpFile &cfile, int depth) const {
	for (int i = 0; i < depth; i++) {
		cfile.writeString("    ");
	}
	cfile.writeString("<!--" + value + "-->");
}

void TiXmlComment::CopyTo(TiXmlComment *target) const {
	TiXmlNode::CopyTo(target);
}

bool TiXmlComment::Accept(TiXmlVisitor *visitor) const {
	return visitor->Visit(*this);
}

TiXmlNode *TiXmlComment::Clone() const {
	TiXmlComment *clone = new TiXmlComment();

	if (!clone)
		return 0;

	CopyTo(clone);
	return clone;
}

void TiXmlText::Print(Common::DumpFile &cfile, int depth) const {
	if (cdata) {
		int i;
		cfile.writeString("\n");
		for (i = 0; i < depth; i++) {
			cfile.writeString("    ");
		}
		cfile.writeString("<![CDATA[" + value + "]]>\n"); // unformatted output
	} else {
		TIXML_STRING buffer;
		PutString(value, &buffer);
		cfile.writeString(buffer);
	}
}

void TiXmlText::CopyTo(TiXmlText *target) const {
	TiXmlNode::CopyTo(target);
	target->cdata = cdata;
}

bool TiXmlText::Accept(TiXmlVisitor *visitor) const {
	return visitor->Visit(*this);
}

TiXmlNode *TiXmlText::Clone() const {
	TiXmlText *clone = 0;
	clone = new TiXmlText("");

	if (!clone)
		return 0;

	CopyTo(clone);
	return clone;
}

TiXmlDeclaration::TiXmlDeclaration(const char *_version,
								   const char *_encoding,
								   const char *_standalone)
	: TiXmlNode(TiXmlNode::DECLARATION) {
	version = _version;
	encoding = _encoding;
	standalone = _standalone;
}

#ifdef TIXML_USE_STL
TiXmlDeclaration::TiXmlDeclaration(const std::string &_version,
								   const std::string &_encoding,
								   const std::string &_standalone)
	: TiXmlNode(TiXmlNode::DECLARATION) {
	version = _version;
	encoding = _encoding;
	standalone = _standalone;
}
#endif

TiXmlDeclaration::TiXmlDeclaration(const TiXmlDeclaration &copy)
	: TiXmlNode(TiXmlNode::DECLARATION) {
	copy.CopyTo(this);
}

void TiXmlDeclaration::operator=(const TiXmlDeclaration &copy) {
	Clear();
	copy.CopyTo(this);
}

void TiXmlDeclaration::Print(Common::DumpFile *cfile, int /*depth*/, TIXML_STRING *str) const {
	if (cfile)
		cfile->writeString("<?xml ");
	if (str)
		(*str) += "<?xml ";

	if (!version.empty()) {
		if (cfile)
			cfile->writeString("version=\"" + version + "\" ");
		if (str) {
			(*str) += "version=\"";
			(*str) += version;
			(*str) += "\" ";
		}
	}
	if (!encoding.empty()) {
		if (cfile)
			cfile->writeString("encoding=\"" + encoding + "\" ");
		if (str) {
			(*str) += "encoding=\"";
			(*str) += encoding;
			(*str) += "\" ";
		}
	}
	if (!standalone.empty()) {
		if (cfile)
			cfile->writeString("standalone=\"" + standalone + "\" ");
		if (str) {
			(*str) += "standalone=\"";
			(*str) += standalone;
			(*str) += "\" ";
		}
	}
	if (cfile)
		cfile->writeString("?>");
	if (str)
		(*str) += "?>";
}

void TiXmlDeclaration::CopyTo(TiXmlDeclaration *target) const {
	TiXmlNode::CopyTo(target);

	target->version = version;
	target->encoding = encoding;
	target->standalone = standalone;
}

bool TiXmlDeclaration::Accept(TiXmlVisitor *visitor) const {
	return visitor->Visit(*this);
}

TiXmlNode *TiXmlDeclaration::Clone() const {
	TiXmlDeclaration *clone = new TiXmlDeclaration();

	if (!clone)
		return 0;

	CopyTo(clone);
	return clone;
}

void TiXmlUnknown::Print(Common::DumpFile &cfile, int depth) const {
	for (int i = 0; i < depth; i++)
		cfile.writeString("    ");
	cfile.writeString("<" + value + ">");
}

void TiXmlUnknown::CopyTo(TiXmlUnknown *target) const {
	TiXmlNode::CopyTo(target);
}

bool TiXmlUnknown::Accept(TiXmlVisitor *visitor) const {
	return visitor->Visit(*this);
}

TiXmlNode *TiXmlUnknown::Clone() const {
	TiXmlUnknown *clone = new TiXmlUnknown();

	if (!clone)
		return 0;

	CopyTo(clone);
	return clone;
}

TiXmlAttributeSet::TiXmlAttributeSet() {
	sentinel.next = &sentinel;
	sentinel.prev = &sentinel;
}

TiXmlAttributeSet::~TiXmlAttributeSet() {
	assert(sentinel.next == &sentinel);
	assert(sentinel.prev == &sentinel);
}

void TiXmlAttributeSet::Add(TiXmlAttribute *addMe) {
#ifdef TIXML_USE_STL
	assert(!Find(TIXML_STRING(addMe->Name()))); // Shouldn't be multiply adding to the set.
#else
	assert(!Find(addMe->Name())); // Shouldn't be multiply adding to the set.
#endif

	addMe->next = &sentinel;
	addMe->prev = sentinel.prev;

	sentinel.prev->next = addMe;
	sentinel.prev = addMe;
}

void TiXmlAttributeSet::Remove(TiXmlAttribute *removeMe) {
	TiXmlAttribute *node;

	for (node = sentinel.next; node != &sentinel; node = node->next) {
		if (node == removeMe) {
			node->prev->next = node->next;
			node->next->prev = node->prev;
			node->next = 0;
			node->prev = 0;
			return;
		}
	}
	assert(0); // we tried to remove a non-linked attribute.
}

#ifdef TIXML_USE_STL
const TiXmlAttribute *TiXmlAttributeSet::Find(const std::string &name) const {
	for (const TiXmlAttribute *node = sentinel.next; node != &sentinel; node = node->next) {
		if (node->name == name)
			return node;
	}
	return 0;
}

/*
TiXmlAttribute*	TiXmlAttributeSet::Find( const std::string& name )
{
	for( TiXmlAttribute* node = sentinel.next; node != &sentinel; node = node->next )
	{
		if ( node->name == name )
			return node;
	}
	return 0;
}
*/
#endif

const TiXmlAttribute *TiXmlAttributeSet::Find(const char *name) const {
	for (const TiXmlAttribute *node = sentinel.next; node != &sentinel; node = node->next) {
		if (strcmp(node->name.c_str(), name) == 0)
			return node;
	}
	return 0;
}

/*
TiXmlAttribute*	TiXmlAttributeSet::Find( const char* name )
{
	for( TiXmlAttribute* node = sentinel.next; node != &sentinel; node = node->next )
	{
		if ( strcmp( node->name.c_str(), name ) == 0 )
			return node;
	}
	return 0;
}
*/

#ifdef TIXML_USE_STL
std::istream &operator>>(std::istream &in, TiXmlNode &base) {
	TIXML_STRING tag;
	tag.reserve(8 * 1000);
	base.StreamIn(&in, &tag);

	base.Parse(tag.c_str(), 0, TIXML_DEFAULT_ENCODING);
	return in;
}
#endif

#ifdef TIXML_USE_STL
std::ostream &operator<<(std::ostream &out, const TiXmlNode &base) {
	TiXmlPrinter printer;
	printer.SetStreamPrinting();
	base.Accept(&printer);
	out << printer.Str();

	return out;
}

std::string &operator<<(std::string &out, const TiXmlNode &base) {
	TiXmlPrinter printer;
	printer.SetStreamPrinting();
	base.Accept(&printer);
	out.append(printer.Str());

	return out;
}
#endif

TiXmlHandle TiXmlHandle::FirstChild() const {
	if (node) {
		TiXmlNode *child = node->FirstChild();
		if (child)
			return TiXmlHandle(child);
	}
	return TiXmlHandle(0);
}

TiXmlHandle TiXmlHandle::FirstChild(const char *value) const {
	if (node) {
		TiXmlNode *child = node->FirstChild(value);
		if (child)
			return TiXmlHandle(child);
	}
	return TiXmlHandle(0);
}

TiXmlHandle TiXmlHandle::FirstChildElement() const {
	if (node) {
		TiXmlElement *child = node->FirstChildElement();
		if (child)
			return TiXmlHandle(child);
	}
	return TiXmlHandle(0);
}

TiXmlHandle TiXmlHandle::FirstChildElement(const char *value) const {
	if (node) {
		TiXmlElement *child = node->FirstChildElement(value);
		if (child)
			return TiXmlHandle(child);
	}
	return TiXmlHandle(0);
}

TiXmlHandle TiXmlHandle::Child(int count) const {
	if (node) {
		int i;
		TiXmlNode *child = node->FirstChild();
		for (i = 0;
			 child && i < count;
			 child = child->NextSibling(), ++i) {
			// nothing
		}
		if (child)
			return TiXmlHandle(child);
	}
	return TiXmlHandle(0);
}

TiXmlHandle TiXmlHandle::Child(const char *value, int count) const {
	if (node) {
		int i;
		TiXmlNode *child = node->FirstChild(value);
		for (i = 0;
			 child && i < count;
			 child = child->NextSibling(value), ++i) {
			// nothing
		}
		if (child)
			return TiXmlHandle(child);
	}
	return TiXmlHandle(0);
}

TiXmlHandle TiXmlHandle::ChildElement(int count) const {
	if (node) {
		int i;
		TiXmlElement *child = node->FirstChildElement();
		for (i = 0;
			 child && i < count;
			 child = child->NextSiblingElement(), ++i) {
			// nothing
		}
		if (child)
			return TiXmlHandle(child);
	}
	return TiXmlHandle(0);
}

TiXmlHandle TiXmlHandle::ChildElement(const char *value, int count) const {
	if (node) {
		int i;
		TiXmlElement *child = node->FirstChildElement(value);
		for (i = 0;
			 child && i < count;
			 child = child->NextSiblingElement(value), ++i) {
			// nothing
		}
		if (child)
			return TiXmlHandle(child);
	}
	return TiXmlHandle(0);
}

bool TiXmlPrinter::VisitEnter(const TiXmlDocument &) {
	return true;
}

bool TiXmlPrinter::VisitExit(const TiXmlDocument &) {
	return true;
}

bool TiXmlPrinter::VisitEnter(const TiXmlElement &element, const TiXmlAttribute *firstAttribute) {
	DoIndent();
	buffer += "<";
	buffer += element.Value();

	for (const TiXmlAttribute *attrib = firstAttribute; attrib; attrib = attrib->Next()) {
		buffer += " ";
		attrib->Print(0, 0, &buffer);
	}

	if (!element.FirstChild()) {
		buffer += " />";
		DoLineBreak();
	} else {
		buffer += ">";
		if (element.FirstChild()->ToText() && element.LastChild() == element.FirstChild() && element.FirstChild()->ToText()->CDATA() == false) {
			simpleTextPrint = true;
			// no DoLineBreak()!
		} else {
			DoLineBreak();
		}
	}
	++depth;
	return true;
}

bool TiXmlPrinter::VisitExit(const TiXmlElement &element) {
	--depth;
	if (!element.FirstChild()) {
		// nothing.
	} else {
		if (simpleTextPrint) {
			simpleTextPrint = false;
		} else {
			DoIndent();
		}
		buffer += "</";
		buffer += element.Value();
		buffer += ">";
		DoLineBreak();
	}
	return true;
}

bool TiXmlPrinter::Visit(const TiXmlText &text) {
	if (text.CDATA()) {
		DoIndent();
		buffer += "<![CDATA[";
		buffer += text.Value();
		buffer += "]]>";
		DoLineBreak();
	} else if (simpleTextPrint) {
		buffer += text.Value();
	} else {
		DoIndent();
		buffer += text.Value();
		DoLineBreak();
	}
	return true;
}

bool TiXmlPrinter::Visit(const TiXmlDeclaration &declaration) {
	DoIndent();
	declaration.Print(0, 0, &buffer);
	DoLineBreak();
	return true;
}

bool TiXmlPrinter::Visit(const TiXmlComment &comment) {
	DoIndent();
	buffer += "<!--";
	buffer += comment.Value();
	buffer += "-->";
	DoLineBreak();
	return true;
}

bool TiXmlPrinter::Visit(const TiXmlUnknown &unknown) {
	DoIndent();
	buffer += "<";
	buffer += unknown.Value();
	buffer += ">";
	DoLineBreak();
	return true;
}
