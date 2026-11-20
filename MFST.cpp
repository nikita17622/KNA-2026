//#include "MFST.h"
//
//namespace MFST
//{
//    //MfstState 
//    MfstState::MfstState()
//    {
//        lenta_position = 0;
//        nrule = -1;
//        nrulechain = -1;
//    }
//
//    MfstState::MfstState(short pposition, MFSTSTSTACK pst, short pnrulechain)
//    {
//        lenta_position = pposition;
//        st = pst;
//        nrulechain = pnrulechain;
//    }
//
//    MfstState::MfstState(short pposition, MFSTSTSTACK pst, short pnrule, short pnrulechain)
//    {
//        lenta_position = pposition;
//        st = pst;
//        nrule = pnrule;
//        nrulechain = pnrulechain;
//    }
//
//    //MfstDiagnosis
//    Mfst::MfstDiagnosis::MfstDiagnosis()
//    {
//        lenta_position = -1;
//        rc_step = SURPRISE;
//        nrule = -1;
//        nrule_chain = -1;
//    }
//
//    Mfst::MfstDiagnosis::MfstDiagnosis(short plenta_position, RC_STEP prc_step, short pnrule, short pnrule_chain)
//    {
//        lenta_position = plenta_position;
//        rc_step = prc_step;
//        nrule = pnrule;
//        nrule_chain = pnrule_chain;
//    }
//
//    // Mfst 
//    Mfst::Mfst()
//    {
//        lenta = 0;
//        lenta_size = lenta_position = 0;
//    }
//
//    Mfst::Mfst(LEX::LEX plex, GRB::Greibach pgrebach)
//    {
//        grebach = pgrebach;
//        lex = plex;
//        lenta = new short[lenta_size = lex.lextable.size];
//        for (int k = 0; k < lenta_size; k++)
//            lenta[k] = TS(lex.lextable.table[k].lexema[0]);
//        lenta_position = 0;
//        st.push(grebach.stbottomT);
//        st.push(grebach.startN);
//        nrulechain = -1;
//    }
//
//    // step
//    Mfst::RC_STEP Mfst::step()
//    {
//        RC_STEP rc = SURPRISE;
//        char lbuf[256], sbuf[256], rbuf[256];
//
//        if (lenta_position < lenta_size)
//        {
//            if (GRB::Rule::Chain::isN(st.top()))
//            {
//                GRB::Rule rule;
//                if ((nrule = grebach.getRule(st.top(), rule)) >= 0)
//                {
//                    GRB::Rule::Chain chain;
//                    if ((nrulechain = rule.getNextChain(lenta[lenta_position], chain, nrulechain + 1)) >= 0)
//                    {
//                        // Трассировка: подбор правила
//                        rule.getCRule(rbuf, nrulechain);
//                        MFST_TRACE(lenta_position, rbuf, getClenta(lbuf, lenta_position), getCSt(sbuf));
//
//                        savestate();
//                        st.pop();
//                        push_chain(chain);
//                        rc = NS_OK;
//                    }
//                    else
//                    {
//                        savediagnosis(NS_NORULECHAIN);
//                        rc = reststate() ? NS_NORULECHAIN : NS_NORULE;
//                    }
//                }
//                else rc = NS_ERROR;
//            }
//            else if ((st.top() == lenta[lenta_position]))
//            {
//                // Трассировка: совпадение терминала
//                std::string t(1, GRB::Rule::Chain::alphabet_to_char(lenta[lenta_position]));
//                MFST_TRACE(lenta_position, t, getClenta(lbuf, lenta_position + 1), getCSt(sbuf));
//
//                lenta_position++;
//                st.pop();
//                nrulechain = -1;
//                rc = TS_OK;
//            }
//            else
//            {
//                savediagnosis(TS_NOK);
//                rc = reststate() ? TS_NOK : NS_NORULECHAIN;
//            }
//        }
//        else rc = LENTA_END;
//        return rc;
//    }
//
//    //  push_chain
//    bool Mfst::push_chain(GRB::Rule::Chain chain)
//    {
//        for (int k = chain.size - 1; k >= 0; k--)
//            st.push(chain.nt[k]);
//        return true;
//    }
//
//    // savestate / reststate 
//    bool Mfst::savestate()
//    {
//        storestate.push(MfstState(lenta_position, st, nrule, nrulechain));
//        return true;
//    }
//
//    bool Mfst::reststate()
//    {
//        bool rc = false;
//        MfstState state;
//        if (rc = (storestate.size() > 0))
//        {
//            state = storestate.top();
//            lenta_position = state.lenta_position;
//            st = state.st;
//            nrule = state.nrule;
//            nrulechain = state.nrulechain;
//            storestate.pop();
//        }
//        return rc;
//    }
//
//    // savediagnosis 
//    bool Mfst::savediagnosis(RC_STEP prc_step)
//    {
//        bool rc = false;
//        short k = 0;
//        while (k < MFST_DIAGN_NUMBER && lenta_position <= diagnosis[k].lenta_position) k++;
//        if (rc = (k < MFST_DIAGN_NUMBER))
//        {
//            diagnosis[k] = MfstDiagnosis(lenta_position, prc_step, nrule, nrulechain);
//            for (short j = k + 1; j < MFST_DIAGN_NUMBER; j++)
//                diagnosis[j].lenta_position = -1;
//        }
//        return rc;
//    }
//
//    // start 
//    bool Mfst::start()
//    {
//        bool rc = false;
//        RC_STEP rc_step = SURPRISE;
//        char buf[MFST_DIAGN_MAXSIZE];
//
//        std::cout << "--- ТРАССИРОВКА ---" << std::endl;
//        MFST_TRACE_START
//
//            rc_step = step();
//        while (rc_step == NS_OK || rc_step == NS_NORULECHAIN ||
//            rc_step == TS_OK || rc_step == TS_NOK)
//            rc_step = step();
//
//        std::cout << std::endl;
//
//        switch (rc_step)
//        {
//        case LENTA_END:
//            std::cout << "---LENTA_END" << std::endl;
//            std::cout << lenta_size << " : всего символов, синтаксический анализ выполнен без ошибок" << std::endl;
//            rc = true;
//            break;
//
//        case NS_NORULE:
//        {
//            std::cout << "---NS_NORULE" << std::endl;
//            short min_pos = 9999;
//            short best_index = -1;
//
//            for (short i = 0; i < MFST_DIAGN_NUMBER; i++)
//            {
//                if (diagnosis[i].lenta_position != -1 && diagnosis[i].lenta_position < min_pos)
//                {
//                    min_pos = diagnosis[i].lenta_position;
//                    best_index = i;
//                }
//            }
//
//            if (best_index != -1)
//                std::cout << getDiagnosis(best_index, buf) << std::endl;
//            break;
//        }
//
//        case NS_NORULECHAIN:
//            std::cout << "---NS_NORULECHAIN" << std::endl;
//            break;
//
//        case NS_ERROR:
//            std::cout << "---NS_ERROR" << std::endl;
//            break;
//
//        default:
//            std::cout << "---SURPRISE" << std::endl;
//            break;
//        }
//        return rc;
//    }
//
//    // getCSt / getClenta 
//    char* Mfst::getCSt(char* buf)
//    {
//        for (int k = (signed)st.size() - 1; k >= 0; --k)
//        {
//            short p = st.c[k];
//            buf[st.size() - 1 - k] = GRB::Rule::Chain::alphabet_to_char(p);
//        }
//        buf[st.size()] = 0x00;
//        return buf;
//    }
//
//    char* Mfst::getClenta(char* buf, short pos, short n)
//    {
//        short i, k = (pos + n < lenta_size) ? pos + n : lenta_size;
//        for (i = pos; i < k; i++)
//            buf[i - pos] = GRB::Rule::Chain::alphabet_to_char(lenta[i]);
//        buf[i - pos] = 0x00;
//        return buf;
//    }
//
//    // getDiagnosis
//    char* Mfst::getDiagnosis(short n, char* buf)
//    {
//        char* rc = nullptr;
//        int errid = 0;
//        int lpos = -1;
//        if (n < MFST_DIAGN_NUMBER && (lpos = diagnosis[n].lenta_position) >= 0)
//        {
//            errid = grebach.getRule(diagnosis[n].nrule).iderror;
//            Error::ERROR err = Error::geterror(errid);
//            sprintf_s(buf, MFST_DIAGN_MAXSIZE, "%d: строка %d, %s",
//                err.id, lex.lextable.table[lpos].sn, err.message);
//            rc = buf;
//        }
//        return rc;
//    }
//
//    //  printrules / savededucation 
//    void Mfst::printrules()
//    {
//        MfstState state;
//        GRB::Rule rule;
//        char buf[200];
//        for (unsigned short k = 0; k < storestate.size(); k++)
//        {
//            state = storestate.c[k];
//            rule = grebach.getRule(state.nrule);
//            std::cout << rule.getCRule(buf, state.nrulechain) << std::endl;
//        }
//    }
//
//    bool Mfst::savededucation()
//    {
//        MfstState state;
//        deducation.nrules = new short[deducation.size = storestate.size()];
//        deducation.nrulechains = new short[deducation.size];
//        for (unsigned short k = 0; k < storestate.size(); k++)
//        {
//            state = storestate.c[k];
//            deducation.nrules[k] = state.nrule;
//            deducation.nrulechains[k] = state.nrulechain;
//        }
//        return true;
//    }
//}