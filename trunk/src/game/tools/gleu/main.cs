using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;

namespace gleu
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class LevelDesigner : System.Windows.Forms.Form
	{
		private System.Windows.Forms.PictureBox picLevel;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;
		private System.Windows.Forms.MenuItem menuItem5;
		private System.Windows.Forms.MenuItem mnuFile;
		private System.Windows.Forms.MenuItem mnuOpen;
		private System.Windows.Forms.MenuItem mnuWindow;
		private System.Windows.Forms.MenuItem mnuSnap;
		private System.Windows.Forms.MenuItem mnuExit;
		private System.Windows.Forms.MainMenu mnuMain;
		private Palette _palette = null;

		public LevelDesigner()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			//
			// TODO: Add any constructor code after InitializeComponent call
			//
			picLevel.Image = System.Drawing.Image.FromFile(@"\\ece-serv-05\schmich\Desktop\starcell_full.png", true);
			picLevel.Height = picLevel.Image.Height;
			picLevel.Width = picLevel.Image.Width;
		}

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
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
			this.picLevel = new System.Windows.Forms.PictureBox();
			this.mnuMain = new System.Windows.Forms.MainMenu();
			this.mnuFile = new System.Windows.Forms.MenuItem();
			this.mnuOpen = new System.Windows.Forms.MenuItem();
			this.menuItem5 = new System.Windows.Forms.MenuItem();
			this.mnuExit = new System.Windows.Forms.MenuItem();
			this.mnuWindow = new System.Windows.Forms.MenuItem();
			this.mnuSnap = new System.Windows.Forms.MenuItem();
			this.SuspendLayout();
			// 
			// picLevel
			// 
			this.picLevel.Location = new System.Drawing.Point(0, 0);
			this.picLevel.Name = "picLevel";
			this.picLevel.TabIndex = 1;
			this.picLevel.TabStop = false;
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
																					this.menuItem5,
																					this.mnuExit});
			this.mnuFile.Text = "&File";
			// 
			// mnuOpen
			// 
			this.mnuOpen.Index = 0;
			this.mnuOpen.Text = "&Open";
			// 
			// menuItem5
			// 
			this.menuItem5.Index = 1;
			this.menuItem5.Text = "-";
			// 
			// mnuExit
			// 
			this.mnuExit.Index = 2;
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
			this.mnuSnap.Index = 0;
			this.mnuSnap.Text = "&Snap";
			this.mnuSnap.Click += new System.EventHandler(this.mnuSnap_Click);
			// 
			// LevelDesigner
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.AutoScroll = true;
			this.ClientSize = new System.Drawing.Size(512, 446);
			this.Controls.Add(this.picLevel);
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.MaximizeBox = false;
			this.Menu = this.mnuMain;
			this.Name = "LevelDesigner";
			this.Text = "GLEU - Graphical Level Editing Utility";
			this.Load += new System.EventHandler(this.onLoad);
			this.Move += new System.EventHandler(this.onMove);
			this.ResumeLayout(false);

		}
		#endregion

		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main() 
		{
			Application.Run(new LevelDesigner());
		}

		private void onLoad(object sender, System.EventArgs e)
		{
			this.Top = 0;

			this.Height = SystemInformation.PrimaryMonitorMaximizedWindowSize.Height
						- 2 * SystemInformation.FrameBorderSize.Height;

			this.Width = picLevel.Width
					   + SystemInformation.VerticalScrollBarWidth 
					   + 2 * SystemInformation.FrameBorderSize.Width;

			this.MinimumSize = new Size(this.Width, 0);
			this.MaximumSize = new Size(this.Width, this.Height);
			
			_palette = new Palette();
			_palette.Show();
			_palette.Location = new Point(this.Width + this.Location.X,
										  this.Location.Y);
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
			this.Location = new Point(this.Location.X, 0);

			if (this.Location.X < 0)
				this.Location = new Point(0, this.Location.Y);

			int extentX = this.Location.X + this.Width + _palette.Width;
			int screenWidth = SystemInformation.PrimaryMonitorSize.Width;
			if (extentX > screenWidth)
				this.Location = new Point(screenWidth - this.Width - _palette.Width, 
					this.Location.Y);
		}
	}
}
