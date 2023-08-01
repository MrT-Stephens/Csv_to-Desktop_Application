#pragma once

#include <format>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <filesystem>

namespace mrt
{
	template <class _StrType>
	struct XML_Attribute
	{
		_StrType m_Name;
		_StrType m_Value;
	};

	template <class _StrType>
	class XML_Node
	{
	private:
		_StrType m_Name;
		_StrType m_Value;
		std::vector<XML_Attribute<_StrType>> m_Attributes;
		std::vector<XML_Node<_StrType>> m_Children;
	public:
		// Constructors
		XML_Node(const _StrType& name, const _StrType& value = _StrType());

		// Copiers & Assignments
		XML_Node(const XML_Node& other);
		XML_Node(XML_Node&& other) noexcept;
		XML_Node& operator=(const XML_Node& other);

		// Member Functions
		_StrType GetName();
		const _StrType& GetName() const;

		_StrType GetValue();
		const _StrType& GetValue() const;

		std::vector<XML_Attribute<_StrType>>::iterator FindAttr(const _StrType& attrName);
		std::vector<XML_Attribute<_StrType>>::const_iterator FindAttr(const _StrType& attrName) const;

		std::vector<XML_Node<_StrType>>::iterator FindChild(const _StrType& childName);
		std::vector<XML_Node<_StrType>>::const_iterator FindChild(const _StrType& childName) const;

		XML_Attribute<_StrType>& AddAttribute(const _StrType& name, const _StrType& value);
		template <class... Args> XML_Attribute<_StrType>& EmplaceAttribute(Args&&... args);

		XML_Node<_StrType>& AddChild(const XML_Node& child);
		template <class... Args> XML_Node<_StrType>& EmplaceChild(Args&&... args);

		XML_Attribute<_StrType>& GetAttribute(const _StrType& name);
		const XML_Attribute<_StrType>& GetAttribute(const _StrType& name) const;

		XML_Node<_StrType>& GetChild(const _StrType& name);
		const XML_Node<_StrType>& GetChild(const _StrType& name) const;

		std::vector<XML_Attribute<_StrType>>& GetAllAttributes();
		const std::vector<XML_Attribute<_StrType>>& GetAllAttributes() const;

		std::vector<XML_Node<_StrType>>& GetAllChildren();
		const std::vector<XML_Node<_StrType>>& GetAllChildren() const;
	};

	enum class XML_Document_FileError
	{
		SUCCESS = 0,
		FAILED_TO_OPEN = 1
	};

	template <class _StrType>
	class XML_Document
	{
	private:
		_StrType m_Version;
		_StrType m_Namespace;
		XML_Node<_StrType> m_Root;
		bool m_AddProlog;
		bool m_NewLines;
	public:
		// Type Aliases
		using ValueType = typename _StrType::value_type;
		using IStream = typename std::basic_istream<ValueType>;
		using OStream = typename std::basic_ostream<ValueType>;
		using OStrStream = typename std::basic_ostringstream<ValueType>;
		using IStrStream = typename std::basic_istringstream<ValueType>;
		using OFstream = typename std::basic_ofstream<ValueType>;
		using IFstream = typename std::basic_ifstream<ValueType>;

		// Constructors
		XML_Document(const XML_Node<_StrType>& root, const _StrType& version, const _StrType& namespaceWord = _StrType(), bool addProlog = true, bool newLines = true);

		// Copiers & Assignments
		XML_Document(const XML_Document<_StrType>& other);
		XML_Document(XML_Document<_StrType>&& other) noexcept;
		XML_Document& operator=(const XML_Document<_StrType>& other);

		// Member Functions
		XML_Node<_StrType>& GetRoot();
		const XML_Node<_StrType>& GetRoot() const;

		_StrType& GetVersion();
		const _StrType& GetVersion() const;

		_StrType& GetNamespace();
		const _StrType& GetNamespace() const;

		bool GetAddProlog() const;

		void SetAddProlog(bool addProlog = true);

		bool GetNewLines() const;

		void SetNewLines(bool newLines = true);

		// Functions made for reading and writing to files. USE THESE IF OUTPUTTING OR INPUTTING TO FILE.
		static XML_Document_FileError ReadDocument(const _StrType& path, XML_Document& document);

		static XML_Document_FileError WriteDocument(const _StrType& path, const XML_Document& document);

		// Recursive called functions to read or writes nodes to input or ouput streams.
		static void WriteChildrenNodes(OStream* fs, const XML_Node<_StrType>& node, bool newLines, size_t tabs = 1);

		static void ReadChildrenNodes(IStream* fs, XML_Node<_StrType>& node);

		// Universal read and write functions. USE THESE IF OUTPUTTING OR INPUTTING TO STRING STREAM, COUT, etc.
		static void ReadDocumentFromStream(IStream* fs, XML_Document& document);

		static void WriteDocumentToStream(OStream* fs, const XML_Document& document);
	};

	template <class _StrType>
	static _StrType getXMLprolog(const _StrType& version);

	template <class _StrType>
	static _StrType getStartNode(const XML_Node<_StrType>& node);

	template <class _StrType>
	static _StrType getEndNode(const XML_Node<_StrType>& node);

	template <class _StrType>
	static _StrType getTabs(size_t amount);

	template <class _StrType>
	static _StrType getNewLine(bool newLines);
}

template <class _StrType>
mrt::XML_Node<_StrType>::XML_Node(const _StrType& name, const _StrType& value) : m_Name(name), m_Value(value) { }

template <class _StrType>
mrt::XML_Node<_StrType>::XML_Node(const XML_Node& other) : m_Name(other.m_Name), m_Value(other.m_Value), m_Attributes(other.m_Attributes), m_Children(other.m_Children) { }

template <class _StrType>
mrt::XML_Node<_StrType>::XML_Node(XML_Node&& other) noexcept :
	m_Name(std::forward<_StrType>(other.m_Name)),
	m_Value(std::forward<_StrType>(other.m_Value)),
	m_Attributes(std::forward<std::vector<XML_Attribute<_StrType>>>(other.m_Attributes)),
	m_Children(std::forward<std::vector<XML_Node<_StrType>>>(other.m_Children)) { }

template <class _StrType>
mrt::XML_Node<_StrType>& mrt::XML_Node<_StrType>::operator=(const XML_Node& other)
{
	m_Name = other.m_Name;
	m_Value = other.m_Value;
	m_Attributes = other.m_Attributes;
	m_Children = other.m_Children;

	return *this;
}

template <class _StrType>
_StrType mrt::XML_Node<_StrType>::GetName() { return m_Name; }

template <class _StrType>
const _StrType& mrt::XML_Node<_StrType>::GetName() const { return m_Name; }

template <class _StrType>
_StrType mrt::XML_Node<_StrType>::GetValue() { return m_Value; }

template <class _StrType>
const _StrType& mrt::XML_Node<_StrType>::GetValue() const { return m_Value; }

template <class _StrType>
std::vector<mrt::XML_Attribute<_StrType>>::iterator mrt::XML_Node<_StrType>::FindAttr(const _StrType& attrName)
{
	return std::find_if(m_Attributes.begin(), m_Attributes.end(), [&attrName](const XML_Attribute<_StrType>& attr) { return attr.m_Name == attrName; });
}

template <class _StrType>
std::vector<mrt::XML_Attribute<_StrType>>::const_iterator mrt::XML_Node<_StrType>::FindAttr(const _StrType& attrName) const
{
	return std::find_if(m_Attributes.cbegin(), m_Attributes.cend(), [&attrName](const XML_Attribute<_StrType>& attr) { return attr.m_Name == attrName; });
}

template <class _StrType>
std::vector<mrt::XML_Node<_StrType>>::iterator mrt::XML_Node<_StrType>::FindChild(const _StrType& childName)
{
	return std::find_if(m_Children.begin(), m_Children.end(), [&childName](const XML_Node<_StrType>& child) { return child.GetName() == childName; });
}

template <class _StrType>
std::vector<mrt::XML_Node<_StrType>>::const_iterator mrt::XML_Node<_StrType>::FindChild(const _StrType& childName) const
{
	return std::find_if(m_Children.cbegin(), m_Children.cend(), [&childName](const XML_Node<_StrType>& child) { return child.GetName() == childName; });
}

template <class _StrType>
mrt::XML_Attribute<_StrType>& mrt::XML_Node<_StrType>::AddAttribute(const _StrType& name, const _StrType& value)
{
	m_Attributes.emplace_back(name, value);

	return m_Attributes.back();
}

template <class _StrType> template <class... Args>
mrt::XML_Attribute<_StrType>& mrt::XML_Node<_StrType>::EmplaceAttribute(Args&&... args)
{
	m_Attributes.emplace_back(std::forward<Args>(args)...);

	return m_Attributes.back();
}

template <class _StrType>
mrt::XML_Node<_StrType>& mrt::XML_Node<_StrType>::AddChild(const XML_Node& child)
{
	m_Children.emplace_back(child);

	return m_Children.back();
}

template <class _StrType> template <class... Args>
mrt::XML_Node<_StrType>& mrt::XML_Node<_StrType>::EmplaceChild(Args&&... args)
{
	m_Children.emplace_back(std::forward<Args>(args)...);

	return m_Children.back();
}

template <class _StrType>
mrt::XML_Attribute<_StrType>& mrt::XML_Node<_StrType>::GetAttribute(const _StrType& name)
{
	typename std::vector<XML_Attribute<_StrType>>::iterator it = FindAttr(name);

	return (it != m_Attributes.end()) ? *it : XML_Attribute<_StrType>();
}

template <class _StrType>
const mrt::XML_Attribute<_StrType>& mrt::XML_Node<_StrType>::GetAttribute(const _StrType& name) const
{
	typename std::vector<XML_Attribute<_StrType>>::const_iterator it = FindAttr(name);

	return (it != m_Attributes.cend()) ? *it : XML_Attribute<_StrType>();
}

template <class _StrType>
mrt::XML_Node<_StrType>& mrt::XML_Node<_StrType>::GetChild(const _StrType& name)
{
	typename std::vector<XML_Node<_StrType>>::iterator it = FindChild(name);

	return (it != m_Children.end()) ? *it : XML_Node<_StrType>();
}

template <class _StrType>
const mrt::XML_Node<_StrType>& mrt::XML_Node<_StrType>::GetChild(const _StrType& name) const
{
	typename std::vector<XML_Node<_StrType>>::const_iterator it = FindChild(name);

	return (it != m_Children.cend()) ? *it : XML_Node<_StrType>();
}

template <class _StrType>
std::vector<mrt::XML_Attribute<_StrType>>& mrt::XML_Node<_StrType>::GetAllAttributes() { return m_Attributes; }

template <class _StrType>
const std::vector<mrt::XML_Attribute<_StrType>>& mrt::XML_Node<_StrType>::GetAllAttributes() const { return m_Attributes; }

template <class _StrType>
std::vector<mrt::XML_Node<_StrType>>& mrt::XML_Node<_StrType>::GetAllChildren() { return m_Children; }

template <class _StrType>
const std::vector<mrt::XML_Node<_StrType>>& mrt::XML_Node<_StrType>::GetAllChildren() const { return m_Children; }



template <class _StrType>
mrt::XML_Document<_StrType>::XML_Document(const XML_Node<_StrType>& root, const _StrType& version, const _StrType& namespaceWord, bool addProlog, bool newLines) :
	m_Root(root), m_Version(version), m_Namespace(namespaceWord), m_AddProlog(addProlog), m_NewLines(newLines) { }

template <class _StrType>
mrt::XML_Document<_StrType>::XML_Document(const XML_Document& other) :
	m_Root(other.m_Root), m_Namespace(other.m_Namespace), m_AddProlog(other.m_AddProlog), m_NewLines(other.m_NewLines) { }

template <class _StrType>
mrt::XML_Document<_StrType>::XML_Document(XML_Document<_StrType>&& other) noexcept :
	m_Root(std::forward<XML_Node<_StrType>>(other.m_Root)),
	m_Namespace(std::forward<_StrType>(other.m_Namespace)),
	m_AddProlog(other.m_AddProlog), m_NewLines(other.m_NewLines) { }

template <class _StrType>
mrt::XML_Document<_StrType>& mrt::XML_Document<_StrType>::operator=(const XML_Document& other)
{
	m_Root = other.m_Root;
	m_Namespace = other.m_Namespace;
	m_AddProlog = other.m_AddProlog;
	m_NewLines = other.m_NewLines;

	return *this;
}

template <class _StrType>
mrt::XML_Node<_StrType>& mrt::XML_Document<_StrType>::GetRoot() { return m_Root; }

template <class _StrType>
const mrt::XML_Node<_StrType>& mrt::XML_Document<_StrType>::GetRoot() const { return m_Root; }

template <class _StrType>
_StrType& mrt::XML_Document<_StrType>::GetVersion() { return m_Version; }

template <class _StrType>
const _StrType& mrt::XML_Document<_StrType>::GetVersion() const { return m_Version; }

template <class _StrType>
_StrType& mrt::XML_Document<_StrType>::GetNamespace() { return m_Namespace; }

template <class _StrType>
const _StrType& mrt::XML_Document<_StrType>::GetNamespace() const { return m_Namespace; }

template <class _StrType>
bool mrt::XML_Document<_StrType>::GetAddProlog() const { return m_AddProlog; }

template <class _StrType>
void mrt::XML_Document<_StrType>::SetAddProlog(bool addProlog) { m_AddProlog = addProlog; }

template <class _StrType>
bool mrt::XML_Document<_StrType>::GetNewLines() const { return m_NewLines; }

template <class _StrType>
void mrt::XML_Document<_StrType>::SetNewLines(bool newLines) { m_NewLines = newLines; }

template <class _StrType>
mrt::XML_Document_FileError mrt::XML_Document<_StrType>::ReadDocument(const _StrType& path, XML_Document& document)
{
	IFstream fs(std::filesystem::path(path), std::ios::in);

	if (!fs.is_open())
	{
		return XML_Document_FileError::FAILED_TO_OPEN;
	}



	return XML_Document_FileError::SUCCESS;
}

template <class _StrType>
mrt::XML_Document_FileError mrt::XML_Document<_StrType>::WriteDocument(const _StrType& path, const XML_Document& document)
{
	OFstream fs(std::filesystem::path(path), std::ios::out);

	if (!fs.is_open())
	{
		return XML_Document_FileError::FAILED_TO_OPEN;
	}

	if (document.GetAddProlog())
	{
		fs << getXMLprolog<_StrType>(document.GetVersion()) << getNewLine<_StrType>(document.GetNewLines());
	}

	fs << getStartNode(document.GetRoot()) << getNewLine<_StrType>(document.GetNewLines());

	WriteChildrenNodes(&fs, document.GetRoot(), document.GetNewLines());

	fs << getEndNode(document.GetRoot()) << getNewLine<_StrType>(document.GetNewLines());

	fs.close();
	return XML_Document_FileError::SUCCESS;
}

template <class _StrType>
void mrt::XML_Document<_StrType>::ReadChildrenNodes(IStream* fs, XML_Node<_StrType>& node)
{

}

template <class _StrType>
void mrt::XML_Document<_StrType>::WriteChildrenNodes(OStream* fs, const XML_Node<_StrType>& node, bool newLines, size_t tabs)
{
	for (const XML_Node<_StrType>& child : node.GetAllChildren())
	{
		if (child.GetAllChildren().size() > 0)
		{
			*fs << (newLines ? getTabs<_StrType>(tabs) : _StrType()) << getStartNode<_StrType>(child) << getNewLine<_StrType>(newLines);

			WriteChildrenNodes(fs, child, newLines, tabs + 1);

			*fs << (newLines ? getTabs<_StrType>(tabs) : _StrType()) << getEndNode<_StrType>(child) << getNewLine<_StrType>(newLines);
		}
		else
		{
			*fs << (newLines ? getTabs<_StrType>(tabs) : _StrType()) << getStartNode<_StrType>(child) << child.GetValue() << getEndNode(child) << getNewLine<_StrType>(newLines);
		}
	}
}

template <class _StrType>
void mrt::XML_Document<_StrType>::ReadDocumentFromStream(IStream* fs, XML_Document& document)
{

}

template <class _StrType>
void mrt::XML_Document<_StrType>::WriteDocumentToStream(OStream* fs, const XML_Document& document)
{
	if (document.GetAddProlog())
	{
		*fs << getXMLprolog<_StrType>(document.GetVersion()) << getNewLine<_StrType>(document.GetNewLines());
	}

	*fs << getStartNode(document.GetRoot()) << getNewLine<_StrType>(document.GetNewLines());

	WriteChildrenNodes(fs, document.GetRoot(), document.GetNewLines());

	*fs << getEndNode(document.GetRoot()) << getNewLine<_StrType>(document.GetNewLines());
}



template <>
std::string mrt::getXMLprolog(const std::string& version)
{
	return std::format("<?xml version=\"{}\" encoding=\"UTF-8\"?>", version);
}

template <>
std::wstring mrt::getXMLprolog(const std::wstring& version)
{
	return std::format(L"<?xml version=\"{}\" encoding=\"UTF-16\"?>", version);
}

template <>
std::u8string mrt::getXMLprolog(const std::u8string& version)
{
	return u8"<?xml version=\"" + version + u8"\" encoding=\"UTF-8\"?>";
}

template <class _StrType>
_StrType mrt::getStartNode(const XML_Node<_StrType>& node)
{
	typename XML_Document<_StrType>::OStrStream ss;

	ss << "<" << node.GetName();

	for (const XML_Attribute<_StrType>& attribute : node.GetAllAttributes())
	{
		ss << " " << attribute.m_Name << "=\"" << attribute.m_Value << "\"";
	}

	ss << ">";

	return ss.str();
}

template <>
std::string mrt::getEndNode(const XML_Node<std::string>& node)
{
	return std::format("</{}>", node.GetName());
}

template <>
std::wstring mrt::getEndNode(const XML_Node<std::wstring>& node)
{
	return std::format(L"</{}>", node.GetName());
}

template <>
std::u8string mrt::getEndNode(const XML_Node<std::u8string>& node)
{
	return u8"</" + node.GetName() + u8">";
}

template <class _StrType>
_StrType mrt::getTabs(size_t amount)
{
	return _StrType(amount, '\t');
}

template <>
std::string mrt::getNewLine(bool newLines)
{
	return newLines ? "\n" : "";
}

template <>
std::wstring mrt::getNewLine(bool newLines)
{
	return newLines ? L"\n" : L"";
}

template <>
std::u8string mrt::getNewLine(bool newLines)
{
	return newLines ? u8"\n" : u8"";
}