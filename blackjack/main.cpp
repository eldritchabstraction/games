#include <stdio.h>
#include <assert.h>

#include <deque>
#include <queue>
#include <algorithm>
#include <thread>
#include <mutex>
#include <chrono>

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
    card_t(int suit, int rank);
    void speak() { printf("%s of %ss with value of %d\n", rank_to_string[rank_], suit_to_string[suit_], value_); }
    int suit_;
    int rank_;
    int value_;
};

card_t::card_t(int suit, int rank): suit_(suit), rank_(rank)
{
    if (rank == ACE)
    {
        value_ = 1;
        return;
    }
    if (rank >= JACK)
    {
        value_ = 10;
        return;
    }
    value_ = rank_;
}

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
        std::mutex mutex_;
};

deck::deck()
{
    for (int it_suit = 0; it_suit < END_SUIT; ++it_suit)
    {
        for (int it_rank = 2; it_rank < END_RANK; ++it_rank)
        {
           card_t *card = new card_t(it_suit, it_rank); 
           deck_.push_back(card);
        }
    }
    random_shuffle(deck_.begin(), deck_.end());
}

card_t* deck::draw()
{
    std::lock_guard<std::mutex> guard(mutex_);
    if (deck_.empty())
    {
        printf("Error: empty deck\n");
        return NULL;
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

class player_t
{
    public:
        player_t(const char *number): points_(0), busted_(0), number_(number) {};
        void draw_cards();
        void return_cards();

        const char *number_;
        int points_;
        int busted_;
        std::mutex mutex_;
        std::queue<card_t *> hand_;
};


void player_t::draw_cards()
{
    // Draw as many cards as it takes to get to 17 points

    while (points_ < 17)
    {
        card_t *card = g_deck.draw();
        points_ += card->value_;
        hand_.push(card);
        printf("player_%s: I drew ", number_);
        card->speak();
        if (points_ > 21)
        {
            printf("player_%s: Busted at %d points!\n", number_, points_);
            busted_++;
            return;
        }
    }

    printf("player_%s: I have %d points\n", number_, points_);
}

void player_t::return_cards()
{
    std::vector<card_t *>::iterator it;

    for (int i = 0; i < hand_.size(); ++i)
    {
        g_deck.insert(hand_.front());
        hand_.pop();
    }
}

void player_draw_thread(player_t *player)
{
    for (int i = 0; i < 50; ++i)
    {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        player->draw_cards();
        player->return_cards();
        player->points_ = 0;
    }
    printf("player_%s: I busted a total of %d times\n", player->number_, player->busted_);
}

void random_test(void)
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

int main (void)
{
    std::thread player_one(player_draw_thread, new player_t("one"));
    std::thread player_two(player_draw_thread, new player_t("two"));

    player_one.join();
    player_two.join();
}
