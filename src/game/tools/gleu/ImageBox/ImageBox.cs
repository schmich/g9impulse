using System;
using System.Collections;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Diagnostics;
using FreeImageAPI;

namespace GLEU
{
	/// <summary>
	/// Summary description for ImageBox.
	/// </summary>
	public class ImageBox : System.Windows.Forms.UserControl
	{
		private System.ComponentModel.Container components = null;

		public ImageBox()
		{
			InitializeComponent();
		}

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

		public bool ImageLoaded
		{
			get
			{
				return _viewImageHandle != 0;
			}
		}
		
		Bitmap _viewImage = null;
		uint _viewImageHandle = 0;

		Rectangle _view;
		public Rectangle View
		{
			get
			{
				return _view;
			}

			set
			{
				_view = value;
				_setView(_view);
			}
		}
		
		Bitmap _origImage = null;
		uint _origImageHandle = 0;

		string _imagePath = "";
		public string ImagePath
		{
			set
			{
				_imagePath = value;
				_origImageHandle = FreeImage.Load(FIF.FIF_PNG, 
												  _imagePath,
												  (int)LoadSaveFlags.PNG_DEFAULT);
				if (_origImageHandle != 0)
				{
					_origImage = _bitmapFromImageHandle(_origImageHandle);
					_setView(_view);
				}
			}
			
			get
			{
				return _imagePath;
			}
		}

		private void _setView(Rectangle view)
		{
			if (_origImageHandle != 0)
			{
				if (view.Bottom != 0)
				{
					_viewImageHandle = _cropImage(_origImageHandle, view);
					_viewImage = _bitmapFromImageHandle(_viewImageHandle);			
				}
				else
				{
					_viewImageHandle = _origImageHandle;
					_viewImage = _origImage;
				}

				this.Width = _viewImage.Width;
				this.Height = _viewImage.Height;
			}
		}
		
		private Bitmap _bitmapFromImageHandle(uint handle)
		{
			Debug.Assert(handle != 0, "Handle is zero!");
			
			int width = (int)FreeImage.GetWidth(handle);
			int height = (int)FreeImage.GetHeight(handle);
			
			Bitmap bmp = new Bitmap(Convert.ToInt32(width), Convert.ToInt32(height));
			Graphics gfx = Graphics.FromImage(bmp);
			IntPtr hdc = gfx.GetHdc();

			SetStretchBltMode(hdc, 3 /* COLORONCOLOR */);
			StretchDIBits(hdc,
				0, 0, width, height,
				0, 0, width, height,
				FreeImage.GetBits(handle),
				FreeImage.GetInfo(handle),
				0 /* DIB_RGB_COLORS */, 0x00CC0020 /* SRCCOPY */);

			gfx.ReleaseHdc(hdc);
			
			return bmp;
		}
		
		private uint _cropImage(uint handle, Rectangle crop)
		{
			return FreeImage.Copy(handle, crop.Left, crop.Top, crop.Right, crop.Bottom);
		}
		
		uint _scaleImage(uint handle, uint scale)
		{
			return FreeImage.Rescale(handle,
				(int)(FreeImage.GetWidth(handle) * scale),
				(int)(FreeImage.GetHeight(handle) * scale),
				FI_FILTER.FILTER_BOX);
		}

		private void onPaint(object sender, System.Windows.Forms.PaintEventArgs e)
		{
			if (_viewImage != null)
				e.Graphics.DrawImage(_viewImage, 0, 0, _viewImage.Width, _viewImage.Height);
		}

		#region Component Designer generated code
		/// <summary> 
		/// Required method for Designer support - do not modify 
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			// 
			// ImageBox
			// 
			this.Name = "ImageBox";
			this.Paint += new System.Windows.Forms.PaintEventHandler(this.onPaint);

		}
		#endregion
		
		[DllImport("gdi32.dll")]
		private static extern int SetStretchBltMode(IntPtr HDC, int iStretchMode);

		[DllImport("gdi32.dll")]
		private static extern int StretchDIBits(
			IntPtr HDC,
			int XDest,                    // x-coord of destination upper-left corner
			int YDest,                    // y-coord of destination upper-left corner
			int nDestWidth,               // width of destination rectangle
			int nDestHeight,              // height of destination rectangle
			int XSrc,                     // x-coord of source upper-left corner
			int YSrc,                     // y-coord of source upper-left corner
			int nSrcWidth,                // width of source rectangle
			int nSrcHeight,               // height of source rectangle
			IntPtr lpBits,				  // bitmap bits
			BITMAPINFO lpBitsInfo,		  // bitmap data
			uint iUsage,                  // usage options
			int dwRop					  // raster operation code
			);
	}
}
