using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using WeifenLuo.WinFormsUI;

namespace GLEU
{
	public class Palette : DockContent
	{
		private GLEU.PictureBoxEx picF16;
		private GLEU.PictureBoxEx picApache;
		private System.ComponentModel.IContainer components;

		public Palette()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			//
			// TODO: Add any constructor code after InitializeComponent call
			//

			picF16.Grayscale = true;
			picApache.Grayscale = true;
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

		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			System.Resources.ResourceManager resources = new System.Resources.ResourceManager(typeof(Palette));
			this.picF16 = new GLEU.PictureBoxEx();
			this.picApache = new GLEU.PictureBoxEx();
			this.SuspendLayout();
			// 
			// picF16
			// 
			this.picF16.Cursor = System.Windows.Forms.Cursors.Hand;
			this.picF16.Grayscale = false;
			this.picF16.Image = ((System.Drawing.Image)(resources.GetObject("picF16.Image")));
			this.picF16.Location = new System.Drawing.Point(8, 8);
			this.picF16.Name = "picF16";
			this.picF16.Size = new System.Drawing.Size(30, 47);
			this.picF16.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
			this.picF16.TabIndex = 0;
			this.picF16.TabStop = false;
			this.picF16.Click += new System.EventHandler(this.onClick);
			this.picF16.MouseEnter += new System.EventHandler(this.onMouseEnter);
			this.picF16.MouseLeave += new System.EventHandler(this.onMouseLeave);
			// 
			// picApache
			// 
			this.picApache.Cursor = System.Windows.Forms.Cursors.Hand;
			this.picApache.Grayscale = false;
			this.picApache.Image = ((System.Drawing.Image)(resources.GetObject("picApache.Image")));
			this.picApache.Location = new System.Drawing.Point(48, 8);
			this.picApache.Name = "picApache";
			this.picApache.Size = new System.Drawing.Size(38, 47);
			this.picApache.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
			this.picApache.TabIndex = 1;
			this.picApache.TabStop = false;
			this.picApache.Click += new System.EventHandler(this.onClick);
			this.picApache.MouseEnter += new System.EventHandler(this.onMouseEnter);
			this.picApache.MouseLeave += new System.EventHandler(this.onMouseLeave);
			// 
			// Palette
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(208, 289);
			this.Controls.Add(this.picApache);
			this.Controls.Add(this.picF16);
			this.DockableAreas = ((WeifenLuo.WinFormsUI.DockAreas)(((((WeifenLuo.WinFormsUI.DockAreas.Float | WeifenLuo.WinFormsUI.DockAreas.DockLeft) 
				| WeifenLuo.WinFormsUI.DockAreas.DockRight) 
				| WeifenLuo.WinFormsUI.DockAreas.DockTop) 
				| WeifenLuo.WinFormsUI.DockAreas.DockBottom)));
			this.DockPadding.Bottom = 3;
			this.DockPadding.Top = 3;
			this.HideOnClose = true;
			this.Name = "Palette";
			this.ShowHint = WeifenLuo.WinFormsUI.DockState.DockLeft;
			this.TabText = "Palette";
			this.Text = "Palette";
			this.ResumeLayout(false);

		}
		#endregion


		private void onMouseEnter(object sender, System.EventArgs e)
		{
			((PictureBoxEx)sender).Grayscale = false;
		}

		private void onMouseLeave(object sender, System.EventArgs e)
		{
			((PictureBoxEx)sender).Grayscale = true;
		}

		private void onClick(object sender, System.EventArgs e)
		{
			//((PictureBoxEx)sender).BackColor = System.Drawing.Color.Black;
		}
	}
}