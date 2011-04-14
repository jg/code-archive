#!/usr/bin/ruby
REPOPATH = '/home/jg/workplace/tuiteraz/'
SERVERLOADDELAY = 10
BROWSERLOADDELAY = 5
WINID= '0x479bc6' #find by xwininfo

def get_hashes
  Dir.chdir(REPOPATH)
  commit_hashes = Array.new and IO.popen('git log').readlines.select {|x| x.scan( /^commit\ (\w{40})$/) {|match| commit_hashes << match} }
  commit_hashes
end

def get_hashes_sample
  commit_hashes = get_hashes
  sample = Array.new and commit_hashes.each_with_index {|obj, i| sample << obj if i % 100 == 0}
  sample
end

#script server process
def run_server
  slave = fork do 
    exec("script/server")
  end
  sleep SERVERLOADDELAY #wait for server to load
  yield
  Process.kill("SIGKILL", slave)
  #puts "Killing #{slave}"
end

def render(page)
  run_server do
    system("firefox 127.0.0.1:3000#{page}")
    sleep BROWSERLOADDELAY #wait for firefox to load the page
  end
end

def make_screenshot(page, fileName)
  render(page)
  system("import -window #{WINID} #{fileName}.png")
end

def switch_to_hash(hash)
  puts "Switching to #{hash}"
  system("git reset --hard #{hash}")
end 


def make_images_from_sample
  sample = get_hashes

  sample.each_with_index do |hash, i|
    #puts "Switching to #{hash}"
    system("git reset --hard #{hash}")
    make_screenshot("/", "root_#{i}")
  end
end

def render_sample
  sample = get_hashes_sample
  sample.each_with_index do |hash, i|
    switch_to_hash(hash)
    render("/")
  end
end

render_sample
