using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;
using WeifenLuo.WinFormsUI;

namespace GLEU
{
	public class MainForm : System.Windows.Forms.Form
	{
        private Level _level = null;

		private Palette _paletteForm = new Palette();
		private Properties _propertiesForm = new Properties();
		private LevelDesigner _designerForm = new LevelDesigner();

		private System.ComponentModel.Container components = null;
		private System.Windows.Forms.MenuItem mnuFile;
		private System.Windows.Forms.MenuItem mnuOpen;
		private System.Windows.Forms.MenuItem mnuExit;
		private System.Windows.Forms.MainMenu mnuMain;
        private System.Windows.Forms.MenuItem mnuSave;
		private ImageBox imgLevel;
		private System.Windows.Forms.MenuItem mnuSeparator0;
		private System.Windows.Forms.StatusBar sbStatus;
		private WeifenLuo.WinFormsUI.DockPanel dockMain;
		private System.Windows.Forms.Panel pnlLevel;
		private System.Windows.Forms.StatusBarPanel sbpnlLocation;

		public MainForm()
		{
			InitializeComponent();

			_paletteForm.Show(dockMain);
			_propertiesForm.Show(dockMain);
			
			_designerForm.Show(dockMain);
			_designerForm.TabText = "Level Design";
		}

		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if (components != null) 
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			System.Resources.ResourceManager resources = new System.Resources.ResourceManager(typeof(MainForm));
			this.mnuMain = new System.Windows.Forms.MainMenu();
			this.mnuFile = new System.Windows.Forms.MenuItem();
			this.mnuOpen = new System.Windows.Forms.MenuItem();
			this.mnuSave = new System.Windows.Forms.MenuItem();
			this.mnuSeparator0 = new System.Windows.Forms.MenuItem();
			this.mnuExit = new System.Windows.Forms.MenuItem();
			this.sbStatus = new System.Windows.Forms.StatusBar();
			this.sbpnlLocation = new System.Windows.Forms.StatusBarPanel();
			this.dockMain = new WeifenLuo.WinFormsUI.DockPanel();
			this.pnlLevel = new System.Windows.Forms.Panel();
			((System.ComponentModel.ISupportInitialize)(this.sbpnlLocation)).BeginInit();
			this.SuspendLayout();
			// 
			// mnuMain
			// 
			this.mnuMain.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
																					this.mnuFile});
			// 
			// mnuFile
			// 
			this.mnuFile.Index = 0;
			this.mnuFile.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
																					this.mnuOpen,
																					this.mnuSave,
																					this.mnuSeparator0,
																					this.mnuExit});
			this.mnuFile.Text = "&File";
			// 
			// mnuOpen
			// 
			this.mnuOpen.Index = 0;
			this.mnuOpen.Shortcut = System.Windows.Forms.Shortcut.CtrlO;
			this.mnuOpen.Text = "&Open...";
			this.mnuOpen.Click += new System.EventHandler(this.mnuOpen_Click);
			// 
			// mnuSave
			// 
			this.mnuSave.Index = 1;
			this.mnuSave.Shortcut = System.Windows.Forms.Shortcut.CtrlS;
			this.mnuSave.Text = "&Save";
			// 
			// mnuSeparator0
			// 
			this.mnuSeparator0.Index = 2;
			this.mnuSeparator0.Text = "-";
			// 
			// mnuExit
			// 
			this.mnuExit.Index = 3;
			this.mnuExit.Shortcut = System.Windows.Forms.Shortcut.CtrlW;
			this.mnuExit.Text = "E&xit";
			this.mnuExit.Click += new System.EventHandler(this.mnuExit_Click);
			// 
			// sbStatus
			// 
			this.sbStatus.Location = new System.Drawing.Point(0, 435);
			this.sbStatus.Name = "sbStatus";
			this.sbStatus.Panels.AddRange(new System.Windows.Forms.StatusBarPanel[] {
																						this.sbpnlLocation});
			this.sbStatus.ShowPanels = true;
			this.sbStatus.Size = new System.Drawing.Size(600, 22);
			this.sbStatus.TabIndex = 3;
			// 
			// dockMain
			// 
			this.dockMain.ActiveAutoHideContent = null;
			this.dockMain.Dock = System.Windows.Forms.DockStyle.Fill;
			this.dockMain.Font = new System.Drawing.Font("Tahoma", 11F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.World);
			this.dockMain.Location = new System.Drawing.Point(0, 0);
			this.dockMain.Name = "dockMain";
			this.dockMain.Size = new System.Drawing.Size(600, 435);
			this.dockMain.TabIndex = 4;
			// 
			// pnlLevel
			// 
			this.pnlLevel.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
				| System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.pnlLevel.AutoScroll = true;
			this.pnlLevel.BackColor = System.Drawing.Color.Black;
			this.pnlLevel.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.pnlLevel.Location = new System.Drawing.Point(-176, 0);
			this.pnlLevel.Name = "pnlLevel";
			this.pnlLevel.Size = new System.Drawing.Size(240, 435);
			this.pnlLevel.TabIndex = 6;
			this.pnlLevel.TabStop = true;
			this.pnlLevel.Visible = false;
			// 
			// MainForm
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(600, 457);
			this.Controls.Add(this.pnlLevel);
			this.Controls.Add(this.dockMain);
			this.Controls.Add(this.sbStatus);
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.IsMdiContainer = true;
			this.Menu = this.mnuMain;
			this.Name = "MainForm";
			this.Text = "GLEU - Graphical Level Editing Utility";
			this.KeyDown += new System.Windows.Forms.KeyEventHandler(this.onKeyDown);
			((System.ComponentModel.ISupportInitialize)(this.sbpnlLocation)).EndInit();
			this.ResumeLayout(false);

		}
		#endregion

		[STAThread]
		static void Main() 
		{
			Application.Run(new MainForm());
		}

		private void mnuExit_Click(object sender, System.EventArgs e)
		{
			Application.Exit();
		}

		private void onKeyDown(object sender, System.Windows.Forms.KeyEventArgs e)
		{
			if (e.KeyCode == Keys.PageDown)
			{
				Point curr = this.AutoScrollPosition;
				this.AutoScrollPosition = new Point(curr.X, -curr.Y + 400);
			}
			else if (e.KeyCode == Keys.PageUp)
			{
				Point curr = this.AutoScrollPosition;
				this.AutoScrollPosition = new Point(curr.X, -curr.Y - 400);
			}
		}

        private void _load(LevelMeta meta)
		{
			imgLevel.ImagePath = meta.BackgroundFilename;
			imgLevel.View = new Rectangle(0, meta.Bounds.Low, 
										  imgLevel.Width, meta.Bounds.High - meta.Bounds.Low);
		}

		private void mnuOpen_Click(object sender, System.EventArgs e)
		{
			FolderBrowserDialog folderDlg = new FolderBrowserDialog();
			folderDlg.Description = "Choose the project directory";
			folderDlg.ShowNewFolderButton = true;
			folderDlg.SelectedPath = Application.StartupPath;
			
			if (folderDlg.ShowDialog() == DialogResult.OK)
			{
				LevelMeta meta;
				string path = folderDlg.SelectedPath;
				try
				{
					meta = new LevelMeta(path + "\\meta.gleu");
				}
				catch (Exception ex)
				{
					MessageBox.Show("Failed to load level project:\n" + ex.Message, 
									"GLEU Load Error", 
									MessageBoxButtons.OK, 
									MessageBoxIcon.Error);
					return;
				}

				_load(meta);
			}
			else
				return;
		}

		private void onMouseMove(object sender, System.Windows.Forms.MouseEventArgs e)
		{
			if (imgLevel.ImageLoaded)
				sbpnlLocation.Text = "(" + e.X.ToString() + ", " + e.Y.ToString() + ")";
		}
	}
}
