# -*- coding: utf-8 -*-

import hadoopy
import zenhan, unicodedata, re
import sys, traceback
import Mykytea

class Normalize_Unicode:

    def __init__(self):        
        self.zenhan = zenhan
        self.kana = zenhan.KANA
        self.digit = zenhan.ASCII|zenhan.DIGIT
        self.re = re
        self.re_c = re.compile('[!-/:-@[-`{-~]')
        self.unicodedata = unicodedata

    def normalize_line(self, data):
        # Input : value, string or unicode, utf-8
        # 正規表現を用い、文字列から半角記号を除去. 記号はスペースに代替
        # Output : value, unicode, utf-8
        if isinstance(data, str):
            value = data.decode('utf-8')
        else:
            value = data
        # following value is unicode
        value = self.zenhan.h2z(value, self.kana)
        value = self.zenhan.z2h(value, self.digit)
        value = self.re.sub(self.re_c, " ", value) #or re.sub(self.re, ' ', value)
        value = value.rstrip(u'\n')
        value = value.lower()
        return value

    def isValid(self, data):
        # word is unicode

        if isinstance(data, str):
            word = data.decode('utf-8')
        else:
            word = data

        # 1文字の単語は登録しない
        if len(word) == 1:
            return False

        # 数字だけの単語は登録しない
        if (self.re.compile(r'[^0-9]')).search(word) is None:
            return False

        # 平仮名二文字の単語は登録しない
        if len(word) == 2 and \
                self.unicodedata.name(word[0])[0:8] == 'HIRAGANA' and \
                self.unicodedata.name(word[1])[0:8] == 'HIRAGANA':
            return False

        # 仮名二文字の単語は登録しない
        if len(word) == 2 and \
                self.unicodedata.name(word[0])[0:8] == 'KATAKANA' and \
                self.unicodedata.name(word[1])[0:8] == 'KATAKANA':
            return False

        ## 英語のみの単語は登録しない
        #if (self.re.compile(r'[^A-Za-z]')).search(word) is None:
        #    return False

        return True

class Mapper(object):

    def __init__(self):
        self.delimiter = " "
        self.condition = "名詞"
        self.NU = Normalize_Unicode()
        self.kytea = Mykytea.Mykytea("model.bin") # make instance for kytea

    def map(self, key, value):
        line = self.NU.normalize_line(value)#line is unicode
        # morphology
        #token = []
        for element in (self.kytea.getTagsToString(line.encode("utf-8"))).split():
            try:
                if element.split('/')[1] == self.condition and \
                        self.NU.isValid( element.split('/')[0] ):
                    #token.append(element.split('/')[0].decode('utf-8'))
                    tmp_key = "%s" % (element.split('/')[0])
                    tmp_value = "%d" % (1)
                    yield tmp_key, tmp_value
            except:
                #sys.stderr.write( traceback.format_exc() + '\n' )
                pass

def combiner(key,values):
    yield key, sum(map( lambda x:int(x), values ))

def reducer(key, values):
    yield key, sum(map( lambda x:int(x), values ))


if __name__ == '__main__':

    hadoopy.run(mapper = Mapper, reducer = reducer, combiner = combiner)
