using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using WeifenLuo.WinFormsUI;

namespace GLEU
{
	public class LevelDesigner : DockContent
	{
		private GLEU.ImageBox imgLevel;
		private System.Windows.Forms.Panel pnlLevel;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public LevelDesigner()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			//
			// TODO: Add any constructor code after InitializeComponent call
			//
		}

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if(components != null)
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		public ImageBox LevelImage
		{
			get
			{
				return imgLevel;
			}
		}

		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.imgLevel = new GLEU.ImageBox();
			this.pnlLevel = new System.Windows.Forms.Panel();
			this.pnlLevel.SuspendLayout();
			this.SuspendLayout();
			// 
			// imgLevel
			// 
			this.imgLevel.ImagePath = "";
			this.imgLevel.Location = new System.Drawing.Point(0, 0);
			this.imgLevel.Name = "imgLevel";
			this.imgLevel.Size = new System.Drawing.Size(56, 150);
			this.imgLevel.TabIndex = 0;
			this.imgLevel.View = new System.Drawing.Rectangle(0, 0, 0, 0);
			// 
			// pnlLevel
			// 
			this.pnlLevel.AutoScroll = true;
			this.pnlLevel.BackColor = System.Drawing.Color.Black;
			this.pnlLevel.Controls.Add(this.imgLevel);
			this.pnlLevel.Dock = System.Windows.Forms.DockStyle.Fill;
			this.pnlLevel.Location = new System.Drawing.Point(0, 3);
			this.pnlLevel.Name = "pnlLevel";
			this.pnlLevel.Size = new System.Drawing.Size(448, 387);
			this.pnlLevel.TabIndex = 1;
			this.pnlLevel.Resize += new System.EventHandler(this.onPanelResize);
			// 
			// LevelDesigner
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(448, 393);
			this.Controls.Add(this.pnlLevel);
			this.DockableAreas = ((WeifenLuo.WinFormsUI.DockAreas)(((((WeifenLuo.WinFormsUI.DockAreas.DockLeft | WeifenLuo.WinFormsUI.DockAreas.DockRight) 
				| WeifenLuo.WinFormsUI.DockAreas.DockTop) 
				| WeifenLuo.WinFormsUI.DockAreas.DockBottom) 
				| WeifenLuo.WinFormsUI.DockAreas.Document)));
			this.DockPadding.Bottom = 3;
			this.DockPadding.Top = 3;
			this.Name = "LevelDesigner";
			this.pnlLevel.ResumeLayout(false);
			this.ResumeLayout(false);

		}
		#endregion

		private void onPanelResize(object sender, System.EventArgs e)
		{
			imgLevel.Left = Math.Max((pnlLevel.Width / 2) - (imgLevel.Width / 2), 0);
		}
	}
}