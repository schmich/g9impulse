using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;

namespace GLEU
{
	public class LevelDesigner : System.Windows.Forms.Form
	{
        private Level _level = null;

		private System.ComponentModel.Container components = null;
		private System.Windows.Forms.MenuItem mnuFile;
		private System.Windows.Forms.MenuItem mnuOpen;
		private System.Windows.Forms.MenuItem mnuWindow;
		private System.Windows.Forms.MenuItem mnuSnap;
		private System.Windows.Forms.MenuItem mnuExit;
		private System.Windows.Forms.MainMenu mnuMain;
        private System.Windows.Forms.MenuItem mnuSave;
		private GLEU.ImageBox imgLevel;
		private System.Windows.Forms.Panel pnlLevel;
		private System.Windows.Forms.MenuItem mnuSeparator0;
		private System.Windows.Forms.StatusBar sbStatus;
		private System.Windows.Forms.StatusBarPanel sbpnlLocation;
		private Palette _palette = null;

		public LevelDesigner()
		{
			InitializeComponent();
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
			System.Resources.ResourceManager resources = new System.Resources.ResourceManager(typeof(LevelDesigner));
			this.mnuMain = new System.Windows.Forms.MainMenu();
			this.mnuFile = new System.Windows.Forms.MenuItem();
			this.mnuOpen = new System.Windows.Forms.MenuItem();
			this.mnuSave = new System.Windows.Forms.MenuItem();
			this.mnuSeparator0 = new System.Windows.Forms.MenuItem();
			this.mnuExit = new System.Windows.Forms.MenuItem();
			this.mnuWindow = new System.Windows.Forms.MenuItem();
			this.mnuSnap = new System.Windows.Forms.MenuItem();
			this.imgLevel = new GLEU.ImageBox();
			this.sbStatus = new System.Windows.Forms.StatusBar();
			this.sbpnlLocation = new System.Windows.Forms.StatusBarPanel();
			this.pnlLevel = new System.Windows.Forms.Panel();
			((System.ComponentModel.ISupportInitialize)(this.sbpnlLocation)).BeginInit();
			this.pnlLevel.SuspendLayout();
			this.SuspendLayout();
			// 
			// mnuMain
			// 
			this.mnuMain.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
																					this.mnuFile,
																					this.mnuWindow});
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
			// mnuWindow
			// 
			this.mnuWindow.Index = 1;
			this.mnuWindow.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
																					  this.mnuSnap});
			this.mnuWindow.Text = "&Window";
			// 
			// mnuSnap
			// 
			this.mnuSnap.Enabled = false;
			this.mnuSnap.Index = 0;
			this.mnuSnap.Shortcut = System.Windows.Forms.Shortcut.CtrlZ;
			this.mnuSnap.Text = "&Snap";
			this.mnuSnap.Click += new System.EventHandler(this.mnuSnap_Click);
			// 
			// imgLevel
			// 
			this.imgLevel.CausesValidation = false;
			this.imgLevel.Cursor = System.Windows.Forms.Cursors.Cross;
			this.imgLevel.ImagePath = "";
			this.imgLevel.Location = new System.Drawing.Point(0, 0);
			this.imgLevel.Name = "imgLevel";
			this.imgLevel.Size = new System.Drawing.Size(0, 0);
			this.imgLevel.TabIndex = 2;
			this.imgLevel.TabStop = false;
			this.imgLevel.View = new System.Drawing.Rectangle(0, 0, 0, 0);
			this.imgLevel.MouseMove += new System.Windows.Forms.MouseEventHandler(this.onMouseMove);
			// 
			// sbStatus
			// 
			this.sbStatus.Location = new System.Drawing.Point(0, 315);
			this.sbStatus.Name = "sbStatus";
			this.sbStatus.Panels.AddRange(new System.Windows.Forms.StatusBarPanel[] {
																						this.sbpnlLocation});
			this.sbStatus.ShowPanels = true;
			this.sbStatus.Size = new System.Drawing.Size(488, 22);
			this.sbStatus.TabIndex = 3;
			// 
			// pnlLevel
			// 
			this.pnlLevel.AutoScroll = true;
			this.pnlLevel.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.pnlLevel.Controls.Add(this.imgLevel);
			this.pnlLevel.Dock = System.Windows.Forms.DockStyle.Fill;
			this.pnlLevel.Location = new System.Drawing.Point(0, 0);
			this.pnlLevel.Name = "pnlLevel";
			this.pnlLevel.Size = new System.Drawing.Size(488, 315);
			this.pnlLevel.TabIndex = 0;
			this.pnlLevel.TabStop = true;
			// 
			// LevelDesigner
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.BackColor = System.Drawing.Color.Black;
			this.ClientSize = new System.Drawing.Size(488, 337);
			this.Controls.Add(this.pnlLevel);
			this.Controls.Add(this.sbStatus);
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.MaximizeBox = false;
			this.Menu = this.mnuMain;
			this.Name = "LevelDesigner";
			this.Text = "GLEU - Graphical Level Editing Utility";
			this.KeyDown += new System.Windows.Forms.KeyEventHandler(this.onKeyDown);
			this.Load += new System.EventHandler(this.onLoad);
			this.Move += new System.EventHandler(this.onMove);
			this.Activated += new System.EventHandler(this.onActivated);
			((System.ComponentModel.ISupportInitialize)(this.sbpnlLocation)).EndInit();
			this.pnlLevel.ResumeLayout(false);
			this.ResumeLayout(false);

		}
		#endregion

		[STAThread]
		static void Main() 
		{
			Application.Run(new LevelDesigner());
		}

		private void onLoad(object sender, System.EventArgs e)
		{
			/*_palette = new Palette();
			_palette.Show();
			_palette.Location = new Point(this.Width + this.Location.X,
										  this.Location.Y);*/
		}

		private void onMove(object sender, System.EventArgs e)
		{
			if (_palette != null)
				_palette.Location = new Point(this.Width + this.Location.X,
											  this.Location.Y);
		}

		private void mnuExit_Click(object sender, System.EventArgs e)
		{
			Application.Exit();
		}

		private void mnuSnap_Click(object sender, System.EventArgs e)
		{
			_snapWindow();
		}
		
		void _snapWindow()
		{
			this.Location = new Point(this.Location.X, 0);
			
			this.Width = imgLevel.Width
					   + SystemInformation.VerticalScrollBarWidth 
					   + 2 * SystemInformation.FrameBorderSize.Width
					   + 2 * SystemInformation.Border3DSize.Width;

			this.Height = SystemInformation.PrimaryMonitorMaximizedWindowSize.Height
						- 2 * SystemInformation.FrameBorderSize.Height;

			if (this.Location.X < 0)
				this.Location = new Point(0, this.Location.Y);

			int extentX = this.Location.X + this.Width;
			if (_palette != null)
				extentX += _palette.Width;

			int screenWidth = SystemInformation.PrimaryMonitorSize.Width;
			if (extentX > screenWidth)
				this.Location = new Point(screenWidth - (extentX - this.Location.X),
					this.Location.Y);

			this.MinimumSize = new Size(this.Width, 0);
			this.MaximumSize = new Size(this.Width, this.Height);

			pnlLevel.Focus();
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

		private bool _alreadyActivated = false;
		private void onActivated(object sender, System.EventArgs e)
		{
			if (_alreadyActivated)
			{
				_alreadyActivated = false;
			}
			else if (_palette != null)
			{
				_alreadyActivated = true;

				_palette.BringToFront();
				this.Focus();
				this.BringToFront();
			}
		}

        private void _load(LevelMeta meta)
		{
			imgLevel.ImagePath = meta.BackgroundFilename;
			imgLevel.View = new Rectangle(0, meta.Bounds.Low, 
										  imgLevel.Width, meta.Bounds.High - meta.Bounds.Low);
			
			mnuSnap.Enabled = true;
			_snapWindow();
		}

		private void mnuOpen_Click(object sender, System.EventArgs e)
		{
			FolderBrowserDialog folderDlg = new FolderBrowserDialog();
			folderDlg.Description = "Choose the project directory";
			
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
