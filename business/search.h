#ifndef SEARCH_H
#define SEARCH_H

#include <vector>
using namespace std;

template <class TBoard> class Search
{
public:
	enum
	{
		SEARCH_STATE_NOT_INITIALIZED,
		SEARCH_STATE_SEARCHING,
		SEARCH_STATE_SUCCEEDED,
		SEARCH_STATE_FAILED,
		SEARCH_STATE_OUT_OF_MEMORY,
		SEARCH_STATE_INVALID
	};

	class Node
	{
		public:

			Node *parent;
			Node *child;

			float g;
			float h;
			float f;

			Node() :
				parent( 0 ),
				child( 0 ),
				g( 0.0f ),
				h( 0.0f ),
				f( 0.0f )
			{
			}

			TBoard board;
	};

    // Untuk mengurutkan elemen heap. Karena di puncak heap adalah yang bernilai paling besar dan kita ingin f terkecil berada di puncak
    // maka pembandingan menggunakan lebih dari (>)

	class HeapCompare_f
	{
		public:

			bool operator() ( const Node *x, const Node *y ) const
			{
				return x->f > y->f;
			}
	};

public:
    Search():
		state( SEARCH_STATE_NOT_INITIALIZED ),
		currentSolutionNode( NULL ),
		cancelRequest( false )
    {
    }
    virtual ~Search() {}

    void CancelSearch()
	{
		cancelRequest = true;
	}

	void SetStartAndGoalStates( TBoard &Start, TBoard &Goal )
	{
		cancelRequest = false;

		start = new Node;
		goal = new (Node);

		start->board = Start;
		goal->board = Goal;

		state = SEARCH_STATE_SEARCHING;

		// inisialisasi state dan nilai node
		initStartNode();

		// Push start node ke openList dan urutkan
		openList.push_back( start );
		initOpenList();

		// Inisialisasi counter langkah
		steps = 0;
	}

	// Majukan pencarian satu langkah
	unsigned int SearchStep()
	{

		// cek apakah pencarian sudah selesai, baik berhasil maupun gagal
		if( (state == SEARCH_STATE_SUCCEEDED) ||
			(state == SEARCH_STATE_FAILED)
		  )
		{
			return state;
		}

		// state masih dalam tahap pencarian, tapi openList sudah kosong
		// maka berpindah ke state gagal
		if( openList.empty() || cancelRequest )
		{
			FreeAllNodes();
			state = SEARCH_STATE_FAILED;
			return state;
		}

		// Majukan counter langkah
		steps ++;

		// Pop node dengan f paling kecil
		Node *n = openList.front(); // ambil pointer ke node
		removeOpenListHead();

		// Cek apakah board sudah sama dengan goal
		if( n->board.IsGoal( goal->board ) )
		{
			// set parent goal sama dengan parent n
			goal->parent = n->parent;

			// backtrack untuk membuat link list solusi
			if( false == n->board.IsSameState( start->board ) )
			{
				delete n; // karena n sudah di representasikan oleh node goal

				// inisialisasi backtrack
				Node *nodeChild = goal;
				Node *nodeParent = goal->parent;

				do
				{
					nodeParent->child = nodeChild;

					nodeChild = nodeParent;
					nodeParent = nodeParent->parent;

				}
				while( nodeChild != start );

			}

			// akah dihapus nanti setelah ditampilkan ke log, karena itu bagian ini di komentari
			//FreeUnusedNodes();

			state = SEARCH_STATE_SUCCEEDED;

			return state;
		}
		else // n belum sama dengan goal
		{
			// ambil kandidat langkah berikutnya
			successors.clear();
			bool ret = n->board.GetSuccessors( this, n->parent ? &n->parent->board : NULL );
			if( !ret ) // gagal alokasi memori, abort
			{
			    typename vector<Node *>::iterator successor;
				for( successor = successors.begin(); successor != successors.end(); successor ++ )
				{
					delete (*successor);
				}
				successors.clear();
				FreeAllNodes();
				state = SEARCH_STATE_OUT_OF_MEMORY;
				return state;
			}

			// proses kandidat langkah berikutnya
			for( typename vector< Node * >::iterator successor = successors.begin(); successor != successors.end(); successor ++ )
			{
				// 	hitung nilai g
				float newg = n->g + n->board.GetCost( (*successor)->board );

                // cari apakah successor sudah ada di openlist
				typename vector< Node * >::iterator openlist_result;
				for( openlist_result = openList.begin(); openlist_result != openList.end(); openlist_result ++ )
				{
					if( (*openlist_result)->board.IsSameState( (*successor)->board ) )
					{
						break;
					}
				}
				if( openlist_result != openList.end() ) // sudah ada di openlist
				{
					if( (*openlist_result)->g <= newg ) // cek apakah nilai g baru malah lebih besar dari yang lama dari board yang sama
					{
						delete (*successor);
						continue; // abaikan successor ini, lanjut ke successor berikutnya
					}
				}

                // cek apakah successor sudah ada di closed list
				typename vector< Node * >::iterator closedlist_result;
				for( closedlist_result = closedList.begin(); closedlist_result != closedList.end(); closedlist_result ++ )
				{
					if( (*closedlist_result)->board.IsSameState( (*successor)->board ) )
					{
						break;
					}
				}
				if( closedlist_result != closedList.end() )
				{
					if( (*closedlist_result)->g <= newg ) // sudah ada di closedList dengan g yang lebih kecil dari g baru
					{
						delete (*successor);
						continue; // maka hapus dan lanjut ke successor berikutnya
					}
				}

				// hitung dan set nilai-nilai di successor node
				initSuccessorNode(*successor, n, newg);

				// keluarkan successor dari closedList jika sudah ada di closedList
				if( closedlist_result != closedList.end() )
				{
					delete (*closedlist_result);
					closedList.erase( closedlist_result );
				}

				// successor dengan board yang sama dengan yang sudah ada di openList akan dimasukkan ke openList, so hapus dulu yang lama
				if( openlist_result != openList.end() )
				{
					delete (*openlist_result);
			   		openList.erase( openlist_result );
			   		prepareOpenListNodeReplacement();
				}

				// masukkan successor ke openList lalu urutkan ulang
				insertNodeToOpenList(*successor);
				afterOpenListNodeInsertion();

			}

            // karena n sudah selesai di proses, masukkan ke closedList
			closedList.push_back( n );

		}

 		return state;

	}

	bool AddSuccessor( TBoard &State )
	{
		Node *node = new Node;

		if( node )
		{
			node->board = State;

			successors.push_back( node );

			return true;
		}

		return false;
	}


	void FreeSolutionNodes()
	{
		Node *n = start;

		if( start->child )
		{
			do
			{
				Node *del = n;
				n = n->child;
				delete del;

				del = NULL;

			} while( n != goal );

			delete n;

		}
		else
		{
			delete start;
			delete goal;
		}

	}


	TBoard *GetSolutionStart()
	{
		currentSolutionNode = start;
		if( start )
		{
			return &start->board;
		}
		else
		{
			return NULL;
		}
	}


	TBoard *GetSolutionNext()
	{
		if( currentSolutionNode )
		{
			if( currentSolutionNode->child )
			{

				Node *child = currentSolutionNode->child;

				currentSolutionNode = currentSolutionNode->child;

				return &child->board;
			}
		}

		return NULL;
	}


	TBoard *GetSolutionEnd()
	{
		currentSolutionNode = goal;
		if( goal )
		{
			return &goal->m_UserState;
		}
		else
		{
			return NULL;
		}
	}


	TBoard *GetSolutionPrev()
	{
		if( currentSolutionNode )
		{
			if( currentSolutionNode->parent )
			{

				Node *parent = currentSolutionNode->parent;

				currentSolutionNode = currentSolutionNode->parent;

				return &parent->m_UserState;
			}
		}

		return NULL;
	}

	int GetStepCount() { return steps; }

	void FreeUnusedNodes()
	{
		typename vector< Node * >::iterator iterOpen = openList.begin();

		while( iterOpen != openList.end() )
		{
			Node *n = (*iterOpen);

			if( !n->child )
			{
				delete n;

				n = NULL;
			}

			iterOpen ++;
		}

		openList.clear();

		typename vector< Node * >::iterator iterClosed;

		for( iterClosed = closedList.begin(); iterClosed != closedList.end(); iterClosed ++ )
		{
			Node *n = (*iterClosed);

			if( !n->child )
			{
				delete n;
				n = NULL;

			}
		}

		closedList.clear();
	}

public:
	vector<Node *> openList;
	vector<Node *> closedList;

protected:
	Node *start;
	Node *goal;

private:
	void FreeAllNodes()
	{
		typename vector< Node * >::iterator iterOpen = openList.begin();

		while( iterOpen != openList.end() )
		{
			Node *n = (*iterOpen);
			delete n;

			iterOpen ++;
		}

		openList.clear();

		typename vector< Node * >::iterator iterClosed;

		for( iterClosed = closedList.begin(); iterClosed != closedList.end(); iterClosed ++ )
		{
			Node *n = (*iterClosed);
			delete n;
		}

		closedList.clear();

		delete goal;
	}

	virtual void initOpenList()
	{
		push_heap( openList.begin(), openList.end(), HeapCompare_f() );
	}

	virtual void removeOpenListHead()
	{
		pop_heap( openList.begin(), openList.end(), HeapCompare_f() ); // urutkan ulang heap tanpa menyertakan node f terkecil, sehingga node f terkecil berpindah ke paling belakang
		openList.pop_back(); // buang node f terkecil yang telah pindah ke paling belakang
	}

	virtual void prepareOpenListNodeReplacement()
	{
        make_heap( openList.begin(), openList.end(), HeapCompare_f() );
	}

	virtual void afterOpenListNodeInsertion()
	{
	    push_heap( openList.begin(), openList.end(), HeapCompare_f() );
	}

	virtual void insertNodeToOpenList(Node *node)
	{
        openList.push_back(node);
	}

	virtual void initStartNode() = 0;
	virtual void initSuccessorNode(Node *successor, Node *parent, unsigned int newg) = 0;


	vector<Node *> successors;

	unsigned int state;

	int steps;

	Node *currentSolutionNode;

	bool cancelRequest;

};

#endif // SEARCH_H
