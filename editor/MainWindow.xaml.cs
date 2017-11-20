using System.Windows;

namespace LanguageEditor
{
    public partial class MainWindow : Window
    {
        public LanguageFile File { get; set; }

        public MainWindow()
        {
            InitializeComponent();
            File = new LanguageFile();
            DataContext = this;
        }

        private void fileOpen_Click(object sender, RoutedEventArgs e)
        {
            var dlg = new Microsoft.Win32.OpenFileDialog()
            {
                Filter = "Language Files (*.language)|*.language|Xml Files (*.xml)|*.xml|All Files (*.*)|*.*"
            };

            if (dlg.ShowDialog() == true)
            {
                if (File.IsOpen())
                {
                    var result = MessageBox.Show("Save language file changes?", "Language Editor", MessageBoxButton.YesNoCancel, MessageBoxImage.Question);
                    switch (result)
                    {
                        case MessageBoxResult.Yes:
                            File.Save();
                            File.Close();
                            File.Open(dlg.FileName);
                            break;
                        case MessageBoxResult.No:
                            File.Close();
                            File.Open(dlg.FileName);
                            break;
                        case MessageBoxResult.Cancel:
                            break;
                    }
                }
                else
                {
                    File.Open(dlg.FileName);
                }
            }
        }

        private void fileClose_Click(object sender, RoutedEventArgs e)
        {
            if (File.IsOpen())
            {
                var result = MessageBox.Show("Save language file changes?", "Language Editor", MessageBoxButton.YesNoCancel, MessageBoxImage.Question);
                switch (result)
                {
                    case MessageBoxResult.Yes:
                        File.Save();
                        File.Close();
                        break;
                    case MessageBoxResult.No:
                        File.Close();
                        break;
                    case MessageBoxResult.Cancel:
                        break;
                }
            }
        }

        private void fileSave_Click(object sender, RoutedEventArgs e)
        {
            File.Save();
        }

        private void fileSaveAs_Click(object sender, RoutedEventArgs e)
        {
            if (File.IsOpen())
            {
                var dlg = new Microsoft.Win32.SaveFileDialog()
                {
                    FileName = File.Path,
                    Filter = "Language Files (*.language)|*.language|Xml Files (*.xml)|*.xml|All Files (*.*)|*.*"
                };

                if (dlg.ShowDialog() == true)
                {
                    File.SaveAs(dlg.FileName);
                }
            }
        }

        private void fileExit_Click(object sender, RoutedEventArgs e)
        {
            if (File.IsOpen())
            {
                var result = MessageBox.Show("Save language file changes?", "Language Editor", MessageBoxButton.YesNoCancel, MessageBoxImage.Question);
                switch (result)
                {
                    case MessageBoxResult.Yes:
                        File.Save();
                        File.Close();
                        Close();
                        break;
                    case MessageBoxResult.No:
                        File.Close();
                        Close();
                        break;
                    case MessageBoxResult.Cancel:
                        break;
                }
            }
            else
            {
                Close();
            }
        }

        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            if (File.IsOpen())
            {
                var result = MessageBox.Show("Save language file changes?", "Language Editor", MessageBoxButton.YesNoCancel, MessageBoxImage.Question);
                switch (result)
                {
                    case MessageBoxResult.Yes:
                        File.Save();
                        break;
                    case MessageBoxResult.No:
                        break;
                    case MessageBoxResult.Cancel:
                        e.Cancel = true;
                        break;
                }
            }
        }
    }
}
