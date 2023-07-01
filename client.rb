require 'socket'
host = 'localhost'
port = 9082
num_requests = 5
num_requests.times do
s = TCPSocket.new(host, port)
s.write("test.txt\n")
s.each_line do |line|
puts line
end
s.close
end
