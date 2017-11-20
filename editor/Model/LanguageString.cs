using System.Xml;

namespace LanguageEditor
{
    public class LanguageString : NotifyObject
    {
        private XmlNode _node;

        public string Key
        {
            get { return _node.Attributes["key"].Value; }
            set
            {
                _node.Attributes["key"].Value = value;
                Notify("Key");
            }
        }

        public string Value
        {
            get { return _node.Attributes["value"].Value; }
            set
            {
                _node.Attributes["value"].Value = value;
                Notify("Value");
            }
        }

        public XmlNode Node
        {
            get { return _node; }
            set
            {
                _node = value;
                Notify("Node");
            }
        }

        public LanguageString(XmlNode node)
        {
            this._node = node;
        }
    }
}
