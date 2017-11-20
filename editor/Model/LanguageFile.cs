using System.Collections.ObjectModel;
using System.Xml;

namespace LanguageEditor
{
    public class LanguageFile : NotifyObject
    {
        private XmlDocument _doc = null;
        private string _path;
        private ObservableCollection<LanguageString> _strings;

        public string Path
        {
            get { return _path; }
            set
            {
                _path = value;
                Notify("Path");
            }
        }

        public string Id
        {
            get
            {
                return _doc != null ? _doc.DocumentElement.Attributes["id"].Value : "";
            }
            set
            {
                if (_doc != null)
                {
                    _doc.DocumentElement.Attributes["id"].Value = value;
                    Notify("Id");
                }
            }
        }

        public string Original
        {
            get
            {
                return _doc != null ? _doc.DocumentElement.Attributes["original"].Value : "";
            }
            set
            {
                if (_doc != null)
                {
                    _doc.DocumentElement.Attributes["original"].Value = value;
                    Notify("Original");
                }
            }
        }

        public string Translated
        {
            get
            {
                return _doc != null ? _doc.DocumentElement.Attributes["translated"].Value : "";
            }
            set
            {
                if (_doc != null)
                {
                    _doc.DocumentElement.Attributes["translated"].Value = value;
                    Notify("Translated");
                }
            }
        }

        public ObservableCollection<LanguageString> Strings
        {
            get { return _strings; }
            set
            {
                _strings = value;
                Notify("Strings");
            }
        }

        public LanguageFile()
        {
            _strings = new ObservableCollection<LanguageString>();
        }

        public bool IsOpen()
        {
            return _doc != null;
        }

        public void Open(string path)
        {
            _doc = new XmlDocument() { PreserveWhitespace = true };
            _doc.Load(path);

            foreach (XmlNode node in _doc.DocumentElement.ChildNodes)
            {
                if (node.Name == "String")
                {
                    Strings.Add(new LanguageString(node));
                }
            }

            Path = path;

            Notify("Id");
            Notify("Original");
            Notify("Translated");
        }

        public void Close()
        {
            Path = null;
            Strings.Clear();
            _doc = null;

            Notify("Id");
            Notify("Original");
            Notify("Translated");
        }

        public void Save()
        {
            if (_doc != null)
            {
                _doc.Save(Path);
            }
        }

        public void SaveAs(string path)
        {
            if (_doc != null)
            {
                _doc.Save(path);
                Path = path;
            }
        }
    }
}
