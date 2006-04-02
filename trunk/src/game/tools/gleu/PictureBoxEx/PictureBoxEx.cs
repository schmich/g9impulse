using System;
using System.Collections;
using System.ComponentModel;
using System.Drawing;
using System.Drawing.Imaging;
using System.Data;
using System.Windows.Forms;

namespace GLEU
{
	public class PictureBoxEx : System.Windows.Forms.PictureBox
	{
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public PictureBoxEx()
		{
			// This call is required by the Windows.Forms Form Designer.
			InitializeComponent();

			// TODO: Add any initialization after the InitComponent call

		}

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if( components != null )
					components.Dispose();
			}
			base.Dispose( disposing );
		}

		Bitmap _origImage = null;
		Bitmap _grayscaleImage = null;

		bool _grayscale = false;
		public bool Grayscale
		{
			get
			{
				return _grayscale;
			}

			set
			{
				_grayscale = value;

				if (base.Image != null)
				{
					if (value)
					{
						if (_grayscaleImage == null)
							_grayscaleImage = _convertToGrayscale(_origImage);

						base.Image = _grayscaleImage;
					}
					else
						base.Image = _origImage;
				}

				Refresh();
			}
		}

		public new Image Image
		{
			get
			{
				return base.Image;
			}

			set
			{
				_origImage = (Bitmap)value;

				if (Grayscale)
				{
					_grayscaleImage = _convertToGrayscale(_origImage);
					base.Image = _grayscaleImage;
				}
				else
					base.Image = _origImage;
			}
		}

		#region Component Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify 
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			// 
			// PictureBoxEx
			// 
			this.Paint += new System.Windows.Forms.PaintEventHandler(this.onPaint);

		}
		#endregion

		public Bitmap _convertToGrayscale(Bitmap source)
		{
			Bitmap gray = new Bitmap(source.Width,source.Height);

			for(int y=0; y < gray.Height; y++)
			{
				for(int x = 0; x < gray.Width; x++)
				{
					Color c = source.GetPixel(x,y);
					if (c.A != 0)
					{
						int lumi = (int)(c.R*0.3 + c.G*0.59+ c.B*0.11);
						gray.SetPixel(x, y, Color.FromArgb(lumi, lumi, lumi));
					}
				}
			}

			return gray;
		}

		private void onPaint(object sender, System.Windows.Forms.PaintEventArgs e)
		{
			if (Image != null)
				e.Graphics.DrawImage(Image, 0, 0);
		}
	}
}
