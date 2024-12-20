#include "macros.h"
#include <array>
#include <string>
extern const std::array<std::string, NUM_SQ> indexToSquare = {
    "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8", "a7", "b7", "c7",
    "d7", "e7", "f7", "g7", "h7", "a6", "b6", "c6", "d6", "e6", "f6",
    "g6", "h6", "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5", "a4",
    "b4", "c4", "d4", "e4", "f4", "g4", "h4", "a3", "b3", "c3", "d3",
    "e3", "f3", "g3", "h3", "a2", "b2", "c2", "d2", "e2", "f2", "g2",
    "h2", "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1"};
// clang-format off

// number of squares that a bishop attacks (not including edges of board since they have no squares behind)
extern const std::array<int, NUM_SQ> bishopRelevantBits = {
  6, 5, 5, 5, 5, 5, 5, 6, 
5, 5, 5, 5, 5, 5, 5, 5, 
5, 5, 7, 7, 7, 7, 5, 5, 
5, 5, 7, 9, 9, 7, 5, 5, 
5, 5, 7, 9, 9, 7, 5, 5, 
5, 5, 7, 7, 7, 7, 5, 5, 
5, 5, 5, 5, 5, 5, 5, 5, 
6, 5, 5, 5, 5, 5, 5, 6 
};

// mask to and the castle flags with every move
extern const std::array<int, NUM_SQ> castleMasks = {
  7, 15, 15, 15, 3, 15, 15, 11,
  15, 15, 15, 15, 15, 15, 15, 15,
  15, 15, 15, 15, 15, 15, 15, 15,
  15, 15, 15, 15, 15, 15, 15, 15,
  15, 15, 15, 15, 15, 15, 15, 15,
  15, 15, 15, 15, 15, 15, 15, 15,
  15, 15, 15, 15, 15, 15, 15, 15,
  13, 15, 15, 15, 12, 15, 15, 14
};

// number of squares that a rook attacks (not including edges of board since they have no squares behind)
extern const std::array<int, NUM_SQ> rookRelevantBits = {
  12, 11, 11, 11, 11, 11, 11, 12, 
11, 10, 10, 10, 10, 10, 10, 11, 
11, 10, 10, 10, 10, 10, 10, 11, 
11, 10, 10, 10, 10, 10, 10, 11, 
11, 10, 10, 10, 10, 10, 10, 11, 
11, 10, 10, 10, 10, 10, 10, 11, 
11, 10, 10, 10, 10, 10, 10, 11, 
12, 11, 11, 11, 11, 11, 11, 12
};

extern const std::array<int, NUM_SQ> squareMirror = {
  a1, b1, c1, d1, e1, f1, g1, h1,
  a2, b2, c2, d2, e2, f2, g2, h2,
  a3, b3, c3, d3, e3, f3, g3, h3,
  a4, b4, c4, d4, e4, f4, g4, h4,
  a5, b5, c5, d5, e5, f5, g5, h5,
  a6, b6, c6, d6, e6, f6, g6, h6,
  a7, b7, c7, d7, e7, f7, g7, h7,
  a8, b8, c8, d8, e8, f8, g8, h8
};

// clang-format on

extern const std::array<int, NUM_PIECES> materialScores = {
    100,   // white pawn score
    300,   // white knight scrore
    350,   // white bishop score
    500,   // white rook score
    900,   // white queen score
    10000, // white king score
    -100,  // black pawn score
    -300,  // black knight scrore
    -350,  // black bishop score
    -500,  // black rook score
    -900,  // black queen score
    -10000 // black king score
};

// convert from character to enum value
extern const std::array<int, ASCII_SZ> charPieces = [] {
  std::array<int, ASCII_SZ> arr{};
  arr['P'] = P;
  arr['N'] = N;
  arr['B'] = B;
  arr['R'] = R;
  arr['Q'] = Q;
  arr['K'] = K;
  arr['p'] = p;
  arr['n'] = n;
  arr['b'] = b;
  arr['r'] = r;
  arr['q'] = q;
  arr['k'] = k;
  return arr;
}();

extern const std::array<std::string, NUM_PIECES> pieceMap = {
    "White Pawn",   "White Knight", "White Bishop", "White Rook",
    "White Queen",  "White King",   "Black Pawn",   "Black Knight",
    "Black Bishop", "Black Rook",   "Black Queen",  "Black King"};

extern const std::string asciiPieces = "PNBRQKpnbrqk";
extern const std::array<std::string, NUM_PIECES> unicodePieces = {
    "♙", "♘", "♗", "♖", "♕", "♔", "♟︎", "♞", "♝", "♜", "♛", "♚"};

extern const std::string emptyPosition = "8/8/8/8/8/8/8/8 w - - 25 60";
extern const std::string startPosition =
    "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
extern const std::string trickyPosition =
    "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq b5 0 1";
extern const std::string killerPosition =
    "rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/P1P1P3/RNBQKBNR w KQkq e6 0 1";
extern const std::string cmkPosition =
    "r2q1rk1/ppp2ppp/2n1bn2/2b1p3/3pP3/3P1NPP/PPP1NPB1/R1BQ1RK1 b - - 0 9";

/* pre-calculated rook magic numbers */
extern const std::array<ull, NUM_SQ> rookMagics = {
    2341876309508014080ULL, 18031991769276416ULL,   36063981659521032ULL,
    2341876788436668416ULL, 144132788852099104ULL,  216173881793711104ULL,
    36029896538980864ULL,   72059934795710598ULL,   9147937288372228ULL,
    4684166099816816640ULL, 6954262062126112768ULL, 397020523387158656ULL,
    1209357787260258304ULL, 1209357787797135872ULL, 5774881918038311168ULL,
    4648137032206434432ULL, 369295721347694592ULL,  2306195677570404354ULL,
    72198881550540808ULL,   292734525811722240ULL,  4803767509778436ULL,
    2468114982655493120ULL, 288234774215168008ULL,  612491748349870340ULL,
    18014675534905376ULL,   306244929817415680ULL,  2322241574404352ULL,
    2269396295229696ULL,    9011608039194752ULL,    281487862137856ULL,
    9011601612997120ULL,    1152921788074754180ULL, 578295012685512752ULL,
    2305878194663723008ULL, 9374383499307388930ULL, 9077602929086976ULL,
    4612811952702358656ULL, 9165546243309600ULL,    144203153368156226ULL,
    4648850613138882636ULL, 141012368392193ULL,     1225120115339690016ULL,
    9024791977689216ULL,    418852357665751168ULL,  8796160163968ULL,
    252205977212846208ULL,  144117455835627528ULL,  13844346731665358866ULL,
    36033469945480000ULL,   70370975556864ULL,      576495936961004800ULL,
    288655921579556992ULL,  1407409277112576ULL,    1153484471748526720ULL,
    90089620204749824ULL,   9296591282112365056ULL, 284301084082306ULL,
    144396740364615810ULL,  70549669777665ULL,      2414496890005487881ULL,
    1297599677281345538ULL, 2306406100977584162ULL, 18577365659584514ULL,
    288525083925301378ULL};

/* pre-calculated bishop magic numbers */
extern const std::array<ull, NUM_SQ> bishopMagics = {
    10134490747900416ULL,    9044584900363264ULL,     4613938385454628868ULL,
    2316064352785956870ULL,  36328139060191496ULL,    869338352325788161ULL,
    181307277255643392ULL,   5068750785155076ULL,     290615271654449ULL,
    792635752902066432ULL,   2305851840207687940ULL,  2612237352908227600ULL,
    54047606594077187ULL,    468375465591046148ULL,   576480545727660032ULL,
    19796050052096ULL,       9232381710044528896ULL,  326651921195663424ULL,
    1190076210592612360ULL,  11272195389014272ULL,    63613637364876288ULL,
    2308657832003666947ULL,  49615505170892800ULL,    1585834455498523648ULL,
    5418544318121984ULL,     2326144394727671458ULL,  2595269928961509889ULL,
    693564238622375976ULL,   2310629184135774212ULL,  589888123044112ULL,
    2343250596297048608ULL,  2306437296460923012ULL,  2398750232109156352ULL,
    2306125927312543824ULL,  865817307703149056ULL,   1157495507878543872ULL,
    10106078114113257504ULL, 603491146699654144ULL,   5009129802234348548ULL,
    2270512986210386ULL,     577624173682102275ULL,   4619005538335851040ULL,
    18049660325216386ULL,    12105684668620277760ULL, 379465660792570368ULL,
    580551268406784ULL,      16465741072275865856ULL, 145277389048512768ULL,
    5356962726351360ULL,     4702322065289510914ULL,  9229002655841386505ULL,
    86835065220763720ULL,    81073726910893092ULL,    288265570221162528ULL,
    578747741925085188ULL,   9043638378055680ULL,     9223953695954307144ULL,
    4503810098597888ULL,     5476400273201072129ULL,  5764642709625242113ULL,
    3413159532708864ULL,     12838672941474648320ULL, 9306336236404884ULL,
    4521265918640144ULL

};
