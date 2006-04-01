using System;

namespace GLEU
{
	public class Level
	{
		public Level(string backgroundFile)
		{
			_backgroundFile = backgroundFile;
		}

		public string BackgroundFile
		{
			get
			{
				return _backgroundFile;
			}
		}

		private string _backgroundFile;
	}
}
