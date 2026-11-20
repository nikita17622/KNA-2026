#include "FST.h"
#include <cstring>
#include <iostream>

namespace FST
{
	RELATION::RELATION(char symbol, short nextNode)
	{
		this->symbol = symbol;
		this->nnode = nextNode;
	}

	NODE::NODE()
	{
		n_relation = 0;
		relations = NULL;
	}

	NODE::NODE(short quantity, RELATION firstRelation, ...)
	{
		n_relation = quantity;
		RELATION* relationsStart = &firstRelation;
		relations = new RELATION[quantity];
		for (short i = 0; i < quantity; i++) { relations[i] = relationsStart[i]; }
	}

	FST::FST(char* string, short quantityStates, NODE firstNode, ...)
	{
		this->string = string;
		nstates = quantityStates;
		NODE* nodeStart = &firstNode;
		nodes = new NODE[quantityStates];
		for (short i = 0; i < quantityStates; i++) { nodes[i] = nodeStart[i]; }

		rstates = new short[quantityStates];
		memset(rstates, 0xFF, sizeof(short) * quantityStates);
		rstates[0] = 0;
		position = -1;
	}

	static bool step(FST& fst, short*& rstates)
	{
		bool rc = false;
		std::swap(rstates, fst.rstates);

		for (short i = 0; i < fst.nstates; i++)
		{
			if (rstates[i] == fst.position)
			{
				if (fst.nodes[i].relations == nullptr) { continue; }

				for (short j = 0; j < fst.nodes[i].n_relation; j++)
				{
					short nn = fst.nodes[i].relations[j].nnode;

					if (nn < 0 || nn >= fst.nstates) { continue; }

					if (fst.nodes[i].relations[j].symbol == fst.string[fst.position])
					{
						fst.rstates[nn] = fst.position + 1;
						rc = true;
					}
				}
			}
		}
		return rc;
	}

	bool execute(FST& fst)
	{
		short* rstates = new short[fst.nstates];
		memset(rstates, 0xFF, sizeof(short) * fst.nstates);
		short lstring = (short)strlen(fst.string);
		bool rc = true;

		for (short i = 0; i < lstring && rc; i++)
		{
			fst.position++;
			rc = step(fst, rstates);
		}

		delete[] rstates;
		return (rc ? (fst.rstates[fst.nstates - 1] == lstring) : rc);
	}
}