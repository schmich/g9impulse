{
module Main where
import Char
import Generator
}

%name parser
%tokentype { Token }

%monad { P } { thenP } { returnP }
%lexer { lexer } { TokenEOF }

%token
    name    { TokenName $$ }
    int     { TokenInt $$ }
    ':'     { TokenColon }
    tab     { TokenTab }

%%

Level
	: Features                  { Level (reverse $1) }

Features
    : {- empty -}               { [] }
    | Features Feature          { $2 : $1 }

Feature
    : Entity                    { $1 }
    | Event                     { $1 }

Entity
    : name Attribs              { Entity $1 (reverse $2) }

Event
    : int Attribs               { Event $1 (reverse $2) }

Attribs
    : Attribs Attrib            { $2 : $1 }
    | Attrib                    { [$1] }

Attrib
    : tab name ':' name         { Attrib $2 $4 }

{
data Token
        = TokenName String
        | TokenInt Int
        | TokenColon
        | TokenTab
        | TokenEOF
    deriving Show

data ParseResult a = Success a | Failed String
type P a = String -> Int -> ParseResult a

thenP :: P a -> (a -> P b) -> P b
m `thenP` k = \s l ->
    case m s l of
        Success a -> k a s l
        Failed e -> Failed e

returnP :: a -> P a
returnP a = \s l -> Success a

failP :: String -> P a
failP e = \s l -> Failed e

catchP :: P a -> (String -> P a) -> P a
catchP m k = \s l ->
    case m s l of
        Success a -> Success a
        Failed e -> k e s l

getLineNum :: P Int
getLineNum = \s l -> Success l

happyError :: P a
happyError = getLineNum `thenP` \line ->
             failP (show line ++ ": parse error")

lexer :: (Token -> P a) -> P a
lexer k s =
    case s of
        [] -> k TokenEOF []
        ('\n':cs) -> (\line -> lexSpace cs (line + 1))
        (c:cs) | isSpace c -> lexer k (dropWhile isSpace cs)
               | isAlpha c -> lexName (c:cs)
               | isDigit c -> lexNum (c:cs)
        (':':cs) -> k TokenColon cs
    where
        lexName cs = let (name, rest) = span isAlpha cs in k (TokenName name) rest
        lexNum cs = let (num, rest) = span isDigit cs in k (TokenInt (read num)) rest
        lexSpace cs =
            let (space, rest) = span (==' ') cs in
                if length space == 4 then k TokenTab rest else lexer k rest

main = do {
    cont <- getContents;
    case parser cont 1 of
        Success lvl -> putStrLn (generateCode lvl)
        Failed why -> error why
}

}
