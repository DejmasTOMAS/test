#include <iostream>

using namespace std;
const int DIRECTIONS = 8;
const int BOARD_SIZE = 15;

enum class Direction
{
	N  	= 0,
	NE 	= 1,
	E  	= 2,
	SE 	= 3,
	S	= 4,
	SW	= 5,
	W	= 6,
	NW	= 7
};
	// opposite is ( dir + DIRECTIONS/2 ) % DIRECTIONS

enum class Owner
{
	X,
	O,
	NONE,
};

class Node
{
public:
	Node( int id );
	Node( Node * n, Direction dir, int id );
	~Node(){};

	Node * 	mExp[DIRECTIONS];
	Owner 	mOwner;
	int 	mID;
};

Node::Node( int id ) : mID(id)
{
	for (int i = 0; i < DIRECTIONS; ++i)
	{
		mExp[i] = nullptr;
	}
}

Node::Node( Node * n, Direction dir, int id ) : mID(id)
{
	for (int i = 0; i < DIRECTIONS; ++i)
	{
		mExp[i] = nullptr;
	}
	mExp[(int)dir] = n;

}

class Graf
{
public:
	Graf();
	~Graf();
	void 	buildGraf();
	void 	showData() const;
	void	bindUp( Node * first, Node * second, Direction dir );
	static int getOpposite( Direction dir );

private:
	Node * mRoot;
	Node * mTable[BOARD_SIZE][BOARD_SIZE];
};

Graf::Graf()
{
	buildGraf();
}

Graf::~Graf()
{
	for (int i = 0; i < BOARD_SIZE; ++i)
	{
		for (int j = 0; j < BOARD_SIZE; ++j)
		{
			delete mTable[i][j];
		}
	}
}

void 	Graf::buildGraf()
{
	for (int i = 0; i < BOARD_SIZE; ++i)
	{
		for (int j = 0; j < BOARD_SIZE; ++j)
		{
			mTable[i][j] = new Node(i*15 + j);
		}
	} // NESW
	for (int i = 0; i < BOARD_SIZE; ++i)
		for (int j = 0; j < BOARD_SIZE; ++j)
		{
			if( j > 0 )
			{
				bindUp( mTable[i][j-1], mTable[i][j], Direction::E ); // j > 0
				if( i > 0  )
					bindUp( mTable[i-1][j-1], mTable[i][j], Direction::SE ); // i > 0 && j > 0
				if( i < 14 )
					bindUp( mTable[i+1][j-1], mTable[i][j], Direction::NE ); // i < 14 && j > 0
			}
			else if( i > 0 )
				bindUp( mTable[i-1][j], mTable[i][j], Direction::S ); // i > 0
		}
	mRoot = mTable[0][0];
}

void 	Graf::showData() const
{
	Node * rowIt, * colIt;
	rowIt = colIt = mRoot;

	for (int i = 0; i < BOARD_SIZE; ++i)
	{	
		for (int j = 0; j < BOARD_SIZE; ++j)
		{
			cout << "( " << i << " @ " << j << " ) -> v tabulce " ;
			cout << mTable[i][j]->mID << ", v grafu " << ((colIt)? colIt->mID : -1 ) << endl;   
			colIt = (colIt)? colIt -> mExp[(int)Direction::E] : nullptr;
		}
		rowIt = rowIt -> mExp[(int)Direction::S];
		colIt = rowIt;
	}
}

void	Graf::bindUp( Node * first, Node * second, Direction dir )
{
	first->mExp[(int)dir] = second;
	second->mExp[getOpposite(dir)] = first;
}

int Graf::getOpposite( Direction dir )
{
	return ( (int)dir + DIRECTIONS/2 ) % DIRECTIONS;
}


int main(int argc, char const *argv[])
{
	Graf g;
	g.showData();
	return 0;
}
