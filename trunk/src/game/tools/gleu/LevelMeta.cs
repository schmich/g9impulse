using System;
using System.IO;
using System.Text.RegularExpressions;
using System.Diagnostics;

namespace GLEU
{
	public struct Range
	{
		public Range(int low, int high)
		{
			Low = low;
			High = high;
		}

		public int Low;
		public int High;
	}

	public class LevelMeta
	{
		public LevelMeta(string metaFile)
		{
			_loadFrom(metaFile);
		}
		
		string _filename = null;
		public string BackgroundFilename
		{
			get
			{
				return _filename;
			}
		}

		private const int INVALID = -1;

		int _top = INVALID;
		int _bottom = INVALID;
		public Range Bounds
		{
			get
			{
				return new Range(_top, _bottom);
			}
		}
		
		private void _loadFrom(string filename)
		{
			if (!File.Exists(filename))
				throw new Exception("File does not exist: " + filename);

			StreamReader file = new StreamReader((System.IO.Stream)File.OpenRead(filename));
			int lineNum = 1;

			string line = file.ReadLine();
			while (line != null)
			{
				Regex imageRe = new Regex(@"^image:\s*(.*)$");
				Regex topRe = new Regex(@"^top:\s*(\d+)\s*$");
				Regex bottomRe = new Regex(@"^bottom:\s*(\d+)\s*$");
				
				MatchCollection matches = imageRe.Matches(line);
				if (matches.Count == 1)
				{
					if (_filename != null)
						throw new Exception("Image file has already been defined, error line " + lineNum.ToString());

					string relFilename = matches[0].Groups[1].Value;
					
					if (Path.IsPathRooted(relFilename))
						_filename = relFilename;
					else
						_filename = Path.GetFullPath(Path.GetDirectoryName(filename) + "\\" + relFilename);
				}
				else
				{
					matches = topRe.Matches(line);
					if (matches.Count == 1)
					{
						if (_top != INVALID)
							throw new Exception("Top has already been defined, error line " + lineNum.ToString());

						_top = int.Parse(matches[0].Groups[1].Value);

						if (_bottom != INVALID && _bottom < _top)
							throw new Exception("Bottom must be greater than top, error line " + lineNum.ToString());
					}
					else
					{
						matches = bottomRe.Matches(line);
						if (matches.Count == 1)
						{
							if (_bottom != INVALID)
								throw new Exception("Bottom has already been defined, error line " + lineNum.ToString());

							_bottom = int.Parse(matches[0].Groups[1].Value);

							if (_top != INVALID && _bottom < _top)
								throw new Exception("Bottom must be greater than top, error line " + lineNum.ToString());
						}
						else
						{
							throw new Exception("Syntax error, line " + lineNum.ToString());
						}
					}
				}
				
				line = file.ReadLine();
				++lineNum;
			}
		}
	}
}
