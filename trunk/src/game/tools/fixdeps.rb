#!/usr/bin/ruby

buildDir = ARGV[0]

while $stdin.gets
    $_.gsub!(/^(.*\.o)/, buildDir + '/\1')
    $_.gsub!(/(\s+)(\w+\.anim\.inl)/, '\1' + buildDir + '/\2')
    $_.gsub!(/(\s+)(\w+\.lvl\.inl)/, '\1' + buildDir + '/\2')
    puts $_
end
