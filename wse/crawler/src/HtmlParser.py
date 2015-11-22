from html.parser import HTMLParser
import re

class MyHTMLParser(HTMLParser):
    def __init__(self):
        HTMLParser.__init__(self)
        self.link = []
        self.page = {}

    def get_link(self):
        return self.link

    def get_page(self):
        return self.page

    def handle_starttag(self, tag, attrs):
        # print ("parsing at here")
        if len(attrs) > 0:
            for attr in attrs:
                if attr[0] == 'href':
                    # print(attr[1])
                    self.link.append(attr[1])

    def handle_data(self, data):
        strinfo = re.compile(',|\(|\)|\{|\}|\[|\]|"')
        str = strinfo.sub(' ', data)
        word_list = re.split(r'[\s]+', str)
        for word in word_list:
            if word in self.page:
                self.page[word] += 1
            else:
                self.page[word] = 1
        # print("Encountered some data  :", data)
