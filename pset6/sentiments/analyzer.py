import nltk

class Analyzer():
    """Implements sentiment analysis."""
    def __init__(self, positives, negatives):
        """Initialize Analyzer."""
        self.positives = set()
        with open("positive-words.txt") as file:
            for line in file:
                if line.startswith(";") == False:
                    self.positives.add(line.rstrip("\n"))
        self.negatives = set()
        with open("negative-words.txt") as file:
            for line in file:
                if line.startswith(";") == False:
                    self.negatives.add(line.rstrip("\n"))

    def analyze(self, text):
        """Analyze text for sentiment, returning its score."""
        tokenizer = nltk.tokenize.TweetTokenizer()
        tokens = tokenizer.tokenize(text)
        counter = 0
        posvalue=0
        negvalue=0
        neuvalue=0
        for counter in range(len(tokens)):
            tokens[counter]=tokens[counter].lower()
            if tokens[counter] in self.positives:
                posvalue = posvalue + 1
            if tokens[counter] in self.negatives:
                negvalue = negvalue + 1
            else:
                neuvalue = neuvalue + 1
        score = posvalue - negvalue
        return score
    