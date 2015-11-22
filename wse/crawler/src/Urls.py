__author__ = 'dongyun'

import functools

@functools.total_ordering
class Urls(object):
    def __init__(self, score, size, depth, url):
        self.score = score
        self.size = size
        self.depth = depth
        self.url = url
        return

    def __gt__(self, other):
        return self.score < getattr(other, 'score')

