#! /usr/bin/ruby

spimpath = "/course/cs031/pro/spim/spim"
mipscopepath = "~/course/cs036/mipscope/code/mipscope"
startline = "Begin tests."
endline = "End tests."


def readUntilSync(fd, sync)
	fd.each { |line|
		break if line.include? sync;
	}

	if fd.eof
		puts "Couldn't find sync line for SPIM input."
		exit
	else
		#puts "Found sync line."
	end
end

file = ARGV[0]

if !file
	puts "Usage: #{$0} <test-file>"
	exit(1)
end

spim = IO.popen("#{spimpath} -file #{file}")
mips = IO.popen("#{mipscopepath} -nox #{file}")

readUntilSync(spim, startline)
readUntilSync(mips, startline)

lastmatch = ""
matches = failures = 0
while !(spim_line = spim.gets).include? endline:

	if spim.eof
		puts "Unexpected end of SPIM stream."
		break
	elsif mips.eof
		puts "Unexpected end of MipScope stream."
		break
	end
	
	if spim_line == (mips_line = mips.gets)
		print  "."
		lastmatch = spim_line
		matches += 1
	else
		puts "Failed on output line #{failures + matches + 1}. Last matched line: '#{lastmatch.strip}'."
		puts "\tSPIM said: #{spim_line}"
		puts "\tMipScope said: #{mips_line}"
		puts
		failures += 1
	end
end

puts "\nMatched #{matches} lines with #{failures} failures."


