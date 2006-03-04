module Generator where
import List

class Emit a where
    emit :: a -> String

data Level = Level [Feature]

instance Emit Level where
	emit = emitLevel

data Feature = Entity String [Attrib]
             | Event Int [Attrib]
    deriving Show

instance Emit Feature where
	emit f = case f of
             Entity _ _ -> emitEntity f
             Event _ _ -> emitEvent f

data Attrib = Attrib String String
    deriving Show

generateCode :: Level -> String
generateCode lvl = emit lvl

emitLevel lvl = join "\n" lines
    where lines = ["static void spawn(World* w, uint16 time)",
                   "{",
                   "switch (time)",
                   "{",
                   concat (map emit (events lvl)),
                   "}",
                   "}"]

emitEvent (Event when _) = "case " ++ show when ++ ":"
                           ++ "\nbreak;\n"

emitEntity _ = "TODO\n"

events (Level fs) = filter isEvent fs
    where isEvent e = case e of 
                      Entity _ _ -> False
                      Event _ _ -> True

join j xs = concat (intersperse j xs)
