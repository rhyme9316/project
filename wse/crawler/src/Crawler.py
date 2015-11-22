__author__ = 'dongyun'

#!/usr/bin/python
#coding:utf-8


import socket
import time
import sys
import datetime
import queue
import Urls
import urllib.request
import re
import urllib.robotparser
import HtmlParser
import urllib.robotparser
import mimetypes
import math

from urllib.robotparser import RobotFileParser
from urllib.parse import urlsplit
from urllib.parse import urljoin

# two global variables
number_of_404_errors = 0
number_of_other_errors = 0

#  download url page and parse it, called by main founction in Crawler
def crawlEngine(url, linkNumber, depth):
    user_agent = 'Mozilla/5.0 (Macintosh; Intel Mac OS X 10_9_4)'
    request = urllib.request.Request(url)
    request.add_header('User-Agent', user_agent)
    request.add_header('Referer', 'https://www.bing.com/')

    global number_of_404_errors
    global number_of_other_errors
    try:
        response = urllib.request.urlopen(request)
        code = response.getcode()
        result = response.read().decode('utf-8')
        page_size = math.ceil(len(result)/1000) # kB

        parser = HtmlParser.MyHTMLParser()
        parser.feed(result)
        (link, data) = parser.get_link(), parser.get_page()

        file = open('%d_%d.html' % (linkNumber, depth),'w')
        file.write(result)

        parser.close()
        file.close()
        return link, data, page_size, code
    except urllib.request.HTTPError as err:
        if err.code == 404:
            number_of_404_errors += 1
            print("404")
            pass
        else:
            number_of_other_errors += 1
    except urllib.request.URLError:
        print("URL Error")
        number_of_other_errors += 1
        pass

    except:
        print("other error")
        number_of_other_errors += 1
        pass

# deal with robot.txt
def robotExclusion(link):
    # print("Robot exclusion.....")
    rp=RobotFileParser();
    rp.set_url(urljoin(link, '/robot.txt'))
    rp.read()
    # print(rp.can_fetch("*", link))
    return rp.can_fetch("*", link)

# judge the link type: images, texts or videos
def judgeTypes(link):
    link_type = mimetypes.guess_type(link, True)[0]
    return link_type

# use number_of_keywords / total_words_number
# I want to use Cosine or BM25 at first and I can
# not find their IDF online.
def getScore(keyword, page_data, stopwords):
    try:
        keyword_list = re.split(r'\+', keyword)
        total_word_num = 0
        keyword_num = 0

        # total number of keywords in page data
        for keyword in keyword_list:
            if keyword in page_data and keyword not in stopwords:
                keyword_num += page_data[keyword]
        # total number of words in page data
        for word in page_data:
            if word not in stopwords:
                total_word_num += page_data[word]

        return keyword_num / total_word_num
    except:
        return 0

def stopWordsTable(filepath):
    stopWords = {}
    for word in open(filepath):
        word = word.strip('\n')
        stopWords[word] = 1

    return stopWords


# main function : input parameter (keywords and n)
def main(argv):
    if len(argv) == 1 or len(argv) >= 9:
        print("Usage: please input keywords with length no longer than 8,\n "
              "and also input number of links you want to download")
        return
    print("Start crawling .............")
    start = time.clock()
    stopwords = stopWordsTable("./stopwords.txt")
    total_page_size = 0
    num_of_files = 0

    # good format keywords in parameters, using '+' to concatenate words
    argv.pop(0)
    n = int(argv.pop(len(argv) - 1))
    keyword = '+'.join(argv)

    # set the beginning url
    root_url = "https://www.bing.com"
    url = root_url + "/search?hl=en&q=%s" % keyword

    # res_file stores the final results (url,original_score, self_score, depth, page_size(kB))
    res_file = open('visited_link_'+ keyword,'w')

    # priority queue stores links waiting for visit
    # and hash table stores visited link with key(host_ip + path)
    pri_q = queue.PriorityQueue(maxsize=-1)
    url_table = {}

    count = 0
    same = 0
    score = 0
    depth = 0
    page_size = 0
    pri_q.put(Urls.Urls(score, page_size, depth, url))

    while pri_q.empty() is False and count < n:
        # get url with maximum score
        url_obj = pri_q.get()
        cur_url = getattr(url_obj, 'url')
        depth = getattr(url_obj, 'depth')

        # get current url's host's ip addr, and
        # concatenate with url's information except host name
        # and this is the url_table's keyword
        # it's used to exclude websites having alias
        try:
            cur_url_split = urlsplit(cur_url)
            path = cur_url_split[2] + cur_url_split[3] + cur_url_split[4]
            myaddr = socket.gethostbyname(cur_url_split[1])
            url_table_key = myaddr + path
        except:
            continue

        # check if cur_url has been visited before, if yes, then same += 1
        if url_table_key in url_table:
            if url_table[url_table_key].score < url_obj.score:
                url_table[url_table_key].score = url_obj.score
            same += 1
            continue

        try:
            count += 1
            # crawl cur_url and return all hyperlink , page data and page size
            (hyperlink_list, page_data, page_size, code) = crawlEngine(cur_url, count, depth)
            download_time = datetime.datetime.now()
            # based on page data and keyword, compute page score
            score = getScore(keyword, page_data, stopwords)
            num_of_files += 1
            total_page_size += page_size
            print('%s\t %f\t %f \t %d\t%d KB\n'%(cur_url,url_obj.score,score,depth, page_size))
            res_file.write('url = %s\t ori_score = %f\t score = %f\t depth = %d\t size = %d KB time = %s\t code = %d\n'%(cur_url,url_obj.score,score,depth, page_size,download_time, code))

            # check each link in page and only stores more related links in priority queue
            # normalized each link
            # solved issue "index.html | htm' or 'index.jsp' or 'main.html"
            # checked robot.txt
            # checked link type: do not parse images or videos
            for link in hyperlink_list:
                for word in argv:
                    match_obj = re.match(r'.*'+word + '.*', link, re.I)  # only keep link which contains keywords
                    if match_obj:
                        tmp_link = urlsplit(link)  # normalize hyperlinks
                        link = urljoin(cur_url, tmp_link.geturl())
                        if True: # robotExclusion(link):
                            type = judgeTypes(link)
                            if type is not None and re.match(r'image.*' or 'video.*', type, re.I):
                                break
                            # if url ends with 'index.html | htm' or 'index.jsp' or 'main.html', then move the those ends
                            link_info = re.compile('(index|main)\.(html|jsp|htm)$')
                            final_link = link_info.sub('', link)
                            # put good links into priority queue
                            pri_q.put(Urls.Urls(score, 0, depth + 1, final_link))
                        break

            # put visited url into dict url_table
            url_table[url_table_key] = Urls.Urls(score, page_size, depth, cur_url)
        except :
            pass

    # calculate total running time
    elapsed = (time.clock() - start)
    total_page_size /= 1000
    global number_of_404_errors
    global number_of_other_errors
    res_file.write('number_of_files = %d\t duplicate_links= %d \t total_size = %f MB, total_time = %f, number_of_404_errors = %d \t number_of_other_errors = %d'
                   %(num_of_files,same, total_page_size, elapsed,number_of_404_errors,number_of_other_errors))
    res_file.close()


# entrance : calling main function
main(sys.argv)

