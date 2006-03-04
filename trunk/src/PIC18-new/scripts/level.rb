#!/usr/bin/ruby

def writeEvents(events)
    eventHandlers = []
    timings = []

    events.each { |event|
        time, commands = event

        timings << time

        eventHandlers << """
        case #{time}:
            #{commands.join("\n            ")}
            break;
        """
    }

    puts """
static void spawn(World* w, uint16 time)
{
    switch (time)
    {
        #{eventHandlers.join("        ")}
    }
}
"""

puts """
static Level theLevel =
{
    NULL, NULL, spawn, 0,
    {
        #{timings.join(",\n        ")},
        0
    }
};
"""
end

def writeInit(name, background)
    upname = name.clone
    upname[/./] = upname[/./].upcase
    while upname.match(/\s+(.)/)
        up = $1.upcase
        upname[/\s+(.)/] = up
    end

    puts """
static bool theLevelInit = false;

static void destroyLevel(Level* level)
{
    destroy(level->background);
    level->currentEvent = 0;
}

Level* create#{upname}(void)
{
    if (!theLevelInit)
    {
        theLevel.destroy = destroyLevel;
        theLevelInit = true;
    }

    theLevel.background = createBackground();
    return &theLevel;
}
"""
end

if ARGV.length < 1
    puts 'Filename required, too few arguments'
    exit(-1)
end

levelName = nil
background = nil
captureBackground = false

time = nil
commands = nil
events = []

filename = ARGV[0]
File.open(filename) { |file|
    lineNum = 1
    file.each_line { |line|
        case line
            when /^\-\-/,/^\s*$/
            when /\s*\[([A-Za-z][\w ]*)\]\s*/
                levelName = $1
                captureBackground = true 
            when /\s*\[(\d+)\]\s*/
                captureBackground = false
                events << [time, commands] if not time.nil?
                time = $1
                commands = []
            when /^\s*(0[xX][0-9A-Fa-f]+|\d+)\s*$/
                if captureBackground
                    background = $1
                else
                    commands << $1.strip
                end
            when /^\s*$/
            when /(.*)/
                if captureBackground
                    puts "Invalid syntax, line #{lineNum}"
                    exit(-1)
                end
                commands << $1.strip
        end

        lineNum = lineNum + 1
    }
}

events << [time, commands] if not time.nil?

events.each { |event|
    time, commands = event
    if commands.empty?
        puts "Event '#{time}' has no commands"
        exit(-1)
    end
}

writeEvents(events)
writeInit(levelName, background)
