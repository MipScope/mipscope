#! /usr/bin/ruby

spimpath = "/course/cs031/pro/spim/spim"

file = ARGV[0]

if !file
	puts "Usage: #{$0} <test-file>"
	exit(1)
end

spim = IO.popen("#{spimpath} -file #{file}")


spim.each { |line|
	puts line
}
