#include <stdio.h>
#include <assert.h>

#include <deque>
#include <algorithm>

typedef enum suit
{
    BEGIN_SUIT = -1,
    DIAMOND = 0,
    CLUB,
    HEART,
    SPADE,
    END_SUIT
} suit;

const char * suit_to_string[4] = {"diamond", "club", "heart", "spade"};

typedef enum rank
{
    BEGIN_RANK = 1,
    TWO = 2,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    NINE,
    TEN,
    JACK,
    QUEEN,
    KING,
    ACE,
    END_RANK
} rank;

const char * rank_to_string[15] = {"zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten",
                                   "jack", "queen", "king", "ace"};

class card_t
{
    public:
    void speak() { printf("%s of %ss\n", rank_to_string[rank_], suit_to_string[suit_]); }
    int suit_;
    int rank_;
};

class deck
{
    public:
        deck();
        card_t* draw();
        void speak();
        void insert(card_t *card);
    private:
        std::deque<card_t*> deck_;
        std::deque<card_t*>::iterator it_;
};

deck::deck()
{
    for (int it_suit = 0; it_suit < END_SUIT; ++it_suit)
    {
        for (int it_rank = 2; it_rank < END_RANK; ++it_rank)
        {
           card_t *card = new card_t; 
           card->suit_ = it_suit;
           card->rank_ = it_rank;
           deck_.push_back(card);
        }
    }
    random_shuffle(deck_.begin(), deck_.end());
}

card_t* deck::draw()
{
    if (deck_.empty())
    {
        printf("Error: empty deck");
        assert(0);
    }
    card_t *card = deck_.front();
    deck_.pop_front();
    return card;
}

void deck::speak()
{
    int i = 1;
    for (it_ = deck_.begin(); it_ != deck_.end(); ++it_)
    {
        printf("%02d: ", i++);
        (*it_)->speak();
    }
}

void deck::insert(card_t *card)
{
    deck_.push_back(card);
    random_shuffle(deck_.begin(), deck_.end());
}
deck g_deck;

int main (void)
{
    g_deck.speak();

    printf("HENRY: drawing 51 cards and only reinserting 40 of them\n");
    for (int i = 0; i < 51; ++i)
    {
        card_t *card = g_deck.draw();
        if (i < 40)
        {
            g_deck.insert(card);
        }
    }


    g_deck.speak();


 
}
