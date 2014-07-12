#include <iostream>
#include <fstream>
#include <ios>

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

class Graph
{
public:
				Graph();
				~Graph();
		void	bindUp( Node * first, Node * second, Direction dir );
		void 	buildGraph();
static	int 	getOpposite( Direction dir );
		bool 	loadGraph( const string& filename );
		void 	showData() const;
		bool	saveGraph( const string& filename ) const;

private:
		Node * 	mRoot;
		Node * 	mTable[BOARD_SIZE][BOARD_SIZE];
};

Graph::Graph()
{
	buildGraph();
}

Graph::~Graph()
{
	for (int i = 0; i < BOARD_SIZE; ++i)
	{
		for (int j = 0; j < BOARD_SIZE; ++j)
		{
			delete mTable[i][j];
		}
	}
}

void 	Graph::buildGraph()
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

void 	Graph::showData() const
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

void	Graph::bindUp( Node * first, Node * second, Direction dir )
{
	first->mExp[(int)dir] = second;
	second->mExp[getOpposite(dir)] = first;
}

int Graph::getOpposite( Direction dir )
{
	return ( (int)dir + DIRECTIONS/2 ) % DIRECTIONS;
}

bool Graph::saveGraph( const string& filename ) const
{
	ofstream	fout;
	fout.open( filename , ios::binary | ios::out );
	if ( fout.fail() )
	{
		cerr << "Cannot to make the file \"" << filename << "\"" << endl;
		return false;
	}

	for (int i = 0; i < BOARD_SIZE; ++i)
	{	
		for (int j = 0; j < BOARD_SIZE; ++j)
		{
			char ch = ( char ) mTable[ i ][ j ] -> mOwner;
			fout.put ( ch );
		}
	}
	return true;
}

bool Graph::loadGraph( const string& filename )
{
	ifstream	fin;
	fin.open( filename , ios::binary | ios::in );
	if ( fin.fail() )
	{
		cerr << "Cannot to read the file \"" << filename << "\"" << endl;
		return false;
	}
	if( mTable[0][0] == nullptr )
		buildGraph( );
	char ch = 0;
	for (int i = 0; i < BOARD_SIZE; ++i)
	{	
		for (int j = 0; j < BOARD_SIZE; ++j)
		{
			if ( ! fin.get ( ch ) )
			{
				cerr << "Some complication with reading the file \"" << filename << "\"" << endl;
				return false;
			}
			mTable[ i ][ j ] -> mOwner = ( Owner )ch;			
		}
	}
	return true;
}


int main( int argc, char const *argv[ ] )
{
	Graph g, h;
	g.showData( );
	if( ! g.saveGraph( "myboard.dat" ) )
		cout << "save fail" << endl;
	else if( ! h.loadGraph( "myboard.dat" ) )
		cout << "load fail" << endl;
	else
		cout << "everything is OK" << endl;

	return 0;
}
