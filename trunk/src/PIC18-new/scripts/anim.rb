#!/usr/bin/ruby

def writeAnim(name, images)
    while name.match(/\s+(.)/)
        up = $1.upcase
        name[/\s+(.)/] = up
    end

    animName = name.clone
    animName[/^./] = animName[/^./].upcase
    animName = "the#{animName}Animation"

    initName = animName.clone
    initName += 'Init'

    imageInit = []
    images.each_with_index { |image, i|
        addr, width, height = image
        imageInit << "#{animName}.images[#{i}] = makeImage(#{addr}, #{width}, #{height});"
    }

    puts "static Animation #{animName};"
    puts "static bool #{initName} = false;"

    puts """
static Animation* #{name}Animation(void)
{
    if (!#{initName})
    {
        #{animName}.numImages = #{images.length};
        #{animName}.images = newArray(Image, #{images.length});

        #{imageInit.join("\n        ")}

        #{initName} = true;
    }

    return &#{animName};
}
    """
end

if ARGV.length < 1
    puts 'Filename required, too few arguments'
    exit(-1)
end

name = nil
images = nil
animations = []

filename = ARGV[0]
File.open(filename) { |file|
    lineNum = 1
    file.each_line { |line|
        case line
            when /\s*\[([A-Za-z][\w ]*)\]\s*/
                animations << [name, images] if not name.nil?
                name = $1
                images = []
            when /\s*(0[xX][0-9A-Fa-f]+|\d+)\s+(0[xX][0-9A-Fa-f]+|\d+)\s+(0[xX][0-9A-Fa-f]+|\d+)\s*/
                addr = $1
                width = $2
                height = $3
                images << [addr, width, height]
            when /^#/,/^\s*$/
            when /.*/
                puts "Invalid syntax, line #{lineNum}"
                exit(-1)
        end
        lineNum = lineNum + 1
    }
}

animations << [name, images] if not name.nil?

animations.each { |animation|
    name, images = animation
    if images.empty?
        puts "Animation '#{name}' has no images"
        exit(-1)
    end
}

animations.each { |animation|
    name, images = animation
    writeAnim(name.strip, images)
}
