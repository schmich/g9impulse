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
		private GLEU.ImageBox imgF16;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public Palette()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			//
			// TODO: Add any constructor code after InitializeComponent call
			//
			imgF16.ImagePath = @"\\ece-serv-05\schmich\Desktop\f16.png";
			imgF16.IsTransparent = true;
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
			this.imgF16 = new GLEU.ImageBox();
			this.SuspendLayout();
			// 
			// imgF16
			// 
			this.imgF16.BackColor = System.Drawing.Color.Lime;
			this.imgF16.Brightness = 100F;
			this.imgF16.Contrast = 100F;
			this.imgF16.Cursor = System.Windows.Forms.Cursors.Hand;
			this.imgF16.ImagePath = "";
			this.imgF16.Location = new System.Drawing.Point(32, 72);
			this.imgF16.Name = "imgF16";
			this.imgF16.Size = new System.Drawing.Size(48, 40);
			this.imgF16.TabIndex = 0;
			this.imgF16.View = new System.Drawing.Rectangle(0, 0, 0, 0);
			this.imgF16.MouseEnter += new System.EventHandler(this.onImageEnter);
			this.imgF16.MouseLeave += new System.EventHandler(this.onImageLeave);
			// 
			// Palette
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.BackColor = System.Drawing.Color.Red;
			this.ClientSize = new System.Drawing.Size(208, 289);
			this.Controls.Add(this.imgF16);
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

		private void onImageEnter(object sender, System.EventArgs e)
		{
			//imgF16.Brightness = 100;
			imgF16.Brightness = -50;
		}

		private void onImageLeave(object sender, System.EventArgs e)
		{
			//imgF16.Brightness = 0;
			imgF16.Brightness = 0;
		}
	}
}