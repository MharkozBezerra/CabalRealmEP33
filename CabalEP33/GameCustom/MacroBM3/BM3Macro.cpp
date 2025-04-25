#include "BM3Macro.h"
#include <fstream>
#include <iostream>
#include <filesystem>
#include "PacketManager/PacketManager.h"
#include <nlohmann/json.hpp>
#include "Basic/Basic.h"
#include "Logs/Logs.h"

std::shared_ptr<BM3Macro> g_pBM3Macro		= std::make_shared<BM3Macro>();

#define INITIAL_SLOT_ATTACK          105

int __cdecl BM3Macro::InitializeStyleEX(S2C_RECEIVESTYLEEX* packet)
{
    VM_STARTV(__FUNCTION__);
    auto orgFunction = ((int(__cdecl*)(S2C_RECEIVESTYLEEX*))0x0099D469);

    if (packet->battleModeIdx == 3)
    {
        C2S_BM3MACROREQUEST request;
        request.Initialize(1);

        PacketManager::Send(::World, &request, sizeof(C2S_BM3MACROREQUEST));
    }

    return orgFunction(packet);
    VM_END;
}

int __cdecl BM3Macro::NfySkillToUser(NFY_SKILLTOUSER* packet)
{
    VM_STARTV(__FUNCTION__);
    auto orgFunction = ((int(__cdecl*)(NFY_SKILLTOUSER*))0x00989810);

    if (packet->wPayloadLen == sizeof(NFY_SKILLTOUSER))
    {
        auto pSkillData = MyUserCtx->GetSkillPtr(packet->wSkillIdx);

        if (MyUserCtx->characterIdx == packet->dwCharacterIdx &&
            pSkillData &&
            pSkillData->iGroup == 32 && 
            pSkillData->iItensity == 3 
        )
        {
            C2S_BM3MACROREQUEST request;
            request.Initialize(packet->bIsUsingBm);

            PacketManager::Send(::World, &request, sizeof(C2S_BM3MACROREQUEST));
        }
    }

    return orgFunction(packet);
    VM_END;
}

void BM3Macro::OnS2CBM3MacroResponse(S2C_HEADER_EX* pS2CHeader, int size, void* unk)
{
    VM_STARTV(__FUNCTION__);
    S2C_BM3MACRORESPONSE* response = (S2C_BM3MACRORESPONSE*)pS2CHeader;
    bool result = response->result;

    g_pBM3Macro->WindowManager(result);

    VM_END;
}

void BM3Macro::LoadConfigFile(std::string filePath, void* buffer, size_t buflen)
{
    VM_STARTV(__FUNCTION__);
    if (Basic::Game::EnsureDir(macroPath))
    {
        std::ifstream load(std::format("{}\\{}.bin", Basic::Game::GetGameDir() + macroPath, MyUserCtx->characterIdx), std::ios_base::binary);

        if (!load.is_open())
        {
            LOG_ERROR("open file error");
            return;
        }
        else
        {
            load.read(reinterpret_cast<char*>(buffer), buflen);

            if (!load.good())
                LOG_ERROR("read file error");
        }

        load.close();
    }
    VM_END;
}

void BM3Macro::SaveConfigFile(std::string filePath, void* buffer, size_t buflen)
{
    VM_STARTV(__FUNCTION__);
    if (Basic::Game::EnsureDir(macroPath))
    {
        std::ofstream save(std::format("{}\\{}.bin", Basic::Game::GetGameDir() + macroPath, MyUserCtx->characterIdx), std::ios_base::binary);

        if (!save.is_open())
        {
            LOG_ERROR("open file error");
        }
        else
        {
            save.write(reinterpret_cast<char*>(buffer), buflen);

            if (!save.good())
                LOG_ERROR("read file error");
        }
        save.close();
    }
    VM_END;
}

void BM3Macro::AttackManager()
{
    USERDATACONTEXT_EXT* pUserCtx   = MyUserCtx;
    double time                     = GAME_TIMER;

    //se está pausado
    if (pause)
        return;

    //ponteiro invalido
    if (!pUserCtx)
        return;

    //não estar de BM3
    if (!(pUserCtx->styleEx & 0x40))
        return;

    //diferente das entidades registradas
    if (pUserCtx->entityType != eEntityTypes::tgt_monster && pUserCtx->entityType != eEntityTypes::tgt_othercharacter && pUserCtx->entityType != eEntityTypes::tgt_dummytraining)
        return;

    //verifica se há uma ação pendente entre ataques, usado para pausar ataques durante buffs
    if (timeActNextTime > time)
        return;

    //verifica se houve tentativa de uso de uma determinada skill durante os ataques do BM3
    if ((pUserCtx->SelectedSkillSlot < 105 || pUserCtx->SelectedSkillSlot > 109) && pUserCtx->isAttacking == true)
    {
        timeActNextTime = time + 0.2f;
        return;
    }

    //o proximo ataque ainda ser maior que o tempo atual
    if (pUserCtx->skillNextTime > time)
        return;

    //não estar de BM3
    if (pUserCtx->actionType != eUserActType::act_idle)
        return;

    //botão esquerdo ou direto pressionados no mapa
    if (pUserCtx->IsLeftBtnPressedInWorld)
        return;

    static int paused = 0;
    
    //ao BM3 ser cancelado por timeout a variavel é setada para -1 (float)
    if (MyUserCtx->BattleModeComboTime == -1.0f)
        ResetAttacks();
            
    if (useFatal)
    {
        if (currentFatalCount < sComboData.fatalCount)
        {
            MyUserCtx->ExecuteSkill(INITIAL_SLOT_ATTACK + currentFatalCount + 2); //+2 pois o id da fatal é 2 + fatalcount
            currentFatalCount++;
        }
        else
        {
            currentFatalCount = 0;
            useFatal = false;
        }
    }
    else
    {
        DWORD tmpSequence = 0;

        if (sComboData.combo[currentSequences - 1] == -1)
        {
            for (int i = (currentSequences - 1); i < 3; i++)
            {
                if (tmpSequence = sComboData.combo[currentSequences - 1])
                    break;
            }
        }
        else
            tmpSequence = sComboData.combo[currentSequences - 1];


        if (tmpSequence != -1)  //checa se a sequência é válida
        {
            //se deu o combo completo seta a flag da fatal
            if (hitCount == 3)
            {
                if (sequences == currentSequences)
                {
                    useFatal = true;
                    currentSequences = 1;
                }
                else
                    currentSequences++;

            }

            int slotIndex = g_pBM3Macro->m_BM3Data[MyUserCtx->iClass].sinergyData[tmpSequence].Attack[hitCount] + INITIAL_SLOT_ATTACK;


            MyUserCtx->ExecuteSkill(g_pBM3Macro->m_BM3Data[MyUserCtx->iClass].sinergyData[tmpSequence].Attack[hitCount] + INITIAL_SLOT_ATTACK);

            hitCount++;

            //reseta a contagem
            hitCount = hitCount >= g_pBM3Macro->MAX_SEQUENCES_COUNT ? 0 : hitCount;
        }
    }
}

void BM3Macro::MinimizeWindow(cSzObjectContext* a, int b, void* c, WINPROC_CLICKIDX clickType)
{
    VM_STARTV(__FUNCTION__);
    //visibilidade das janelas
    g_pBM3Macro->obj_MainBoxMaximized.SetVisible(false);
    g_pBM3Macro->obj_MainBoxMinimized.SetVisible(true);
    g_pBM3Macro->obj_MaximizeBtn.SetVisible(true);
    g_pBM3Macro->obj_MinimizeBtn.SetVisible(false);

    //fatal buttons
    for (auto& it : g_pBM3Macro->obj_RadioButton_Fatal)
    {
        it.SetVisible(false);
    }

    //seta os radios como visiveis caso exista titulo da sinergia
    for (int i = 0; i < 4; i++)
    {
        g_pBM3Macro->obj_RadioButton[i][0].SetVisible(false);
        g_pBM3Macro->obj_RadioButton[i][1].SetVisible(false);
        g_pBM3Macro->obj_RadioButton[i][2].SetVisible(false);
        g_pBM3Macro->obj_RadioButton[i][3].SetVisible(false);
        g_pBM3Macro->obj_AddImages[i].SetVisible(false);
        g_pBM3Macro->txt_Descriptions[i].SetVisible(false);
        g_pBM3Macro->txt_SequenceName[i].SetVisible(false);
        g_pBM3Macro->txt_SinergyIndex[i][0].SetVisible(false);
        g_pBM3Macro->txt_SinergyIndex[i][1].SetVisible(false);
        g_pBM3Macro->txt_SinergyIndex[i][2].SetVisible(false);
        g_pBM3Macro->txt_SinergyIndex[i][3].SetVisible(false);
    }

    //desabilita os textos das fatais
    g_pBM3Macro->txt_FatalIndex[0].SetVisible(false);
    g_pBM3Macro->txt_FatalIndex[1].SetVisible(false);
    g_pBM3Macro->txt_FatalIndex[2].SetVisible(false);
    VM_END;
}

void BM3Macro::MaximizeWindow(cSzObjectContext* a, int b, void* c, WINPROC_CLICKIDX clickType)
{
    VM_STARTV(__FUNCTION__);
    int iClass = MyUserCtx->iClass;

    if (!g_pBM3Macro->m_BM3Data.contains(iClass))
        return;

    auto& mapRef = g_pBM3Macro->m_BM3Data[iClass];

    g_pBM3Macro->obj_MainBoxMaximized.SetVisible(true);
    g_pBM3Macro->obj_MainBoxMinimized.SetVisible(true);
    g_pBM3Macro->obj_MaximizeBtn.SetVisible(false);
    g_pBM3Macro->obj_MinimizeBtn.SetVisible(true);

    //seta os radios como visiveis caso exista titulo da sinergia
    for (int i = 0; i < 4; i++)
    {
        bool result = mapRef.sinergyData[i].IsValid();

        g_pBM3Macro->obj_RadioButton[i][0].SetVisible(result);
        g_pBM3Macro->obj_RadioButton[i][1].SetVisible(result);
        g_pBM3Macro->obj_RadioButton[i][2].SetVisible(result);

        g_pBM3Macro->txt_SinergyIndex[i][0].SetVisible(result);
        g_pBM3Macro->txt_SinergyIndex[i][1].SetVisible(result);
        g_pBM3Macro->txt_SinergyIndex[i][2].SetVisible(result);

        g_pBM3Macro->obj_AddImages[i].SetVisible(result);

        g_pBM3Macro->txt_Descriptions[i].SetVisible(result);
        g_pBM3Macro->txt_SequenceName[i].SetVisible(result);
    }

    //habilita os textos das fatais
    g_pBM3Macro->txt_FatalIndex[0].SetVisible(true);
    g_pBM3Macro->txt_FatalIndex[1].SetVisible(true);
    g_pBM3Macro->txt_FatalIndex[2].SetVisible(true);

    //se não tiver botões selecionados, marca o da fatal 1 como selecionado
    int selected = 0;
    for (auto& it : g_pBM3Macro->obj_RadioButton_Fatal)
    {
        it.SetVisible(true);

        if (it.IsSelected())
            selected++;
    }
    if (selected == 0)
    {
        g_pBM3Macro->obj_RadioButton_Fatal[0].SetSelected(true);
        g_pBM3Macro->sComboData.fatalCount = 1;
    }
    VM_END;
}

void BM3Macro::MountDescription()
{

}

void BM3Macro::WindowManager(bool status)
{
    VM_STARTV(__FUNCTION__);
    if (status)
    {
        //reseta os botões de forma obrigatória
        for (int i = 0; i < 4; i++)
        {
            for (int ii = 0; ii < 3; ii++)
            {
                g_pBM3Macro->obj_RadioButton[i][ii].SetSelected(false);
            }
        }

        //reseta o button visibility solucionando problema de duplo cliques ao alterar entre sinergias com mais de 3
        for (int i = 0; i < g_pBM3Macro->MAX_SEQUENCES_COUNT; i++)
        {
            g_pBM3Macro->sComboData.btnVisibility[i][0] = 0;
            g_pBM3Macro->sComboData.btnVisibility[i][1] = 0;
            g_pBM3Macro->sComboData.btnVisibility[i][2] = 0;
        }

        //reseta os botões das fatais
        for (int i = 0; i < 3; i++)
        {
            g_pBM3Macro->obj_RadioButton_Fatal[i].SetSelected(false);
        }

        //reseta as configurações anteriores
        for (int i = 0; i < 3; i++)
        {
            g_pBM3Macro->sComboData.combo[i] = -1;
        }

        //carrega as configurações
        g_pBM3Macro->LoadConfigFile(macroPath, &g_pBM3Macro->sComboData, sizeof(g_pBM3Macro->sComboData));

        //seta a seleção nos botões
        for (int i = 0; i < 4; i++)
        {
            for (int ii = 0; ii < 3; ii++)
            {
                g_pBM3Macro->obj_RadioButton[i][ii].SetSelected(false);
                g_pBM3Macro->obj_RadioButton[i][ii].SetSelected(g_pBM3Macro->sComboData.btnVisibility[i][ii] ? true : false);
            }
        }

        //seta a seleção do fatal
        if (g_pBM3Macro->sComboData.fatalCount)
        {
            g_pBM3Macro->obj_RadioButton_Fatal[g_pBM3Macro->sComboData.fatalCount - 1].SetSelected(true);
        }

        //PEGA A QUANTIDADE DE SEQUENCIAS MARCADAS E PASSA PARA UM CONTADOR
        int counter = 0;
        for (int i = 0; i < g_pBM3Macro->MAX_SEQUENCES_COUNT; i++)
        {
            for (int ii = 0; ii < g_pBM3Macro->MAX_SYNERGY_COUNT; ii++)
            {

                switch (ii)
                {
                case 0:
                    if (g_pBM3Macro->obj_RadioButton[i][ii].IsSelected())
                        counter += 1;
                    break;
                case 1:
                    if (g_pBM3Macro->obj_RadioButton[i][ii].IsSelected())
                        counter += 2;
                    break;
                case 2:
                    if (g_pBM3Macro->obj_RadioButton[i][ii].IsSelected())
                        counter += 3;
                    break;
                default:
                    break;
                }
            }
        }
        g_pBM3Macro->sequences = counter;

        //coloca a janela na ultima posição salva
        g_pBM3Macro->obj_MainBoxMinimized.SetPositionL(g_pBM3Macro->sComboData.uiPosition.x, g_pBM3Macro->sComboData.uiPosition.y);

        g_pBM3Macro->ResetAttacks();

        g_pBM3Macro->Present();
    }

    g_pBM3Macro->ObjVisible = status;
    VM_END;
}

void BM3Macro::OnSelectSynergyOptions(cSzObjectContext* pObjectCtx, int b, void* c, WINPROC_CLICKIDX clickType)
{
    VM_STARTV(__FUNCTION__);
    int iClass          = MyUserCtx->iClass;
    int row             = pObjectCtx->rowIdx;
    int col             = pObjectCtx->colIdx;

    int selectCounter = 0;

    //DESMARCA TODOS OS BOTÕES DA SEQUENCIA SELECIONADA
    for (int i = 0; i < g_pBM3Macro->MAX_SYNERGY_COUNT; i++)
    {
        g_pBM3Macro->obj_RadioButton[row][i].SetSelected(false);
    
        if (i != col)
        {
            g_pBM3Macro->sComboData.btnVisibility[row][i] = 0;
        }
        else
        {
            g_pBM3Macro->sComboData.btnVisibility[row][i] = g_pBM3Macro->sComboData.btnVisibility[row][i] == 0 ? 1 : 0;
        }
    }
    
    //MARCA O BOTÃO SELECIONADO
    g_pBM3Macro->obj_RadioButton[row][col].SetSelected(g_pBM3Macro->sComboData.btnVisibility[row][col]);
    
    
    //PEGA A QUANTIDADE DE SEQUENCIAS MARCADAS E PASSA PARA UM CONTADOR
    for (int i = 0; i < g_pBM3Macro->MAX_SEQUENCES_COUNT; i++)
    {
        for (int ii = 0; ii < g_pBM3Macro->MAX_SYNERGY_COUNT; ii++)
        {
    
            switch (ii)
            {
            case 0:
                if (g_pBM3Macro->obj_RadioButton[i][ii].IsSelected())
                    selectCounter += 1;
                break;
            case 1:
                if (g_pBM3Macro->obj_RadioButton[i][ii].IsSelected())
                    selectCounter += 2;
                break;
            case 2:
                if (g_pBM3Macro->obj_RadioButton[i][ii].IsSelected())
                    selectCounter += 3;
                break;
            default:
                break;
            }
        }
    }
    
    //DESMARCA TODOS CASO SEJA MAIOR QUE 3 SELEÇÕES
    if (selectCounter > 3)
    {
        for (int i = 0; i < g_pBM3Macro->MAX_SEQUENCES_COUNT; i++)
        {
            for (int ii = 0; ii < g_pBM3Macro->MAX_SYNERGY_COUNT; ii++)
            {
                g_pBM3Macro->obj_RadioButton[i][ii].SetSelected(false);
                if (i != row && ii != col)
                {
                    g_pBM3Macro->sComboData.btnVisibility[i][ii] = 0;
                }
            }
        }
    
        //LIMPA O BUFFER DE ATAQUES
        for (int i = 0; i < g_pBM3Macro->MAX_SYNERGY_COUNT; i++)
        {
            //reseta todos os ataques
            g_pBM3Macro->sComboData.combo[i] = -1;
        }
    
        //reseta o button visibility solucionando problema de duplo cliques ao alterar entre sinergias com mais de 3
        for (int i = 0; i < g_pBM3Macro->MAX_SEQUENCES_COUNT; i++)
        {
            if (i != row)
            {
                g_pBM3Macro->sComboData.btnVisibility[i][0] = 0;
                g_pBM3Macro->sComboData.btnVisibility[i][1] = 0;
                g_pBM3Macro->sComboData.btnVisibility[i][2] = 0;
            }
        }
    }
    
    //MARCA O BOTÃO SELECIONADO
    g_pBM3Macro->obj_RadioButton[row][col].SetSelected(g_pBM3Macro->sComboData.btnVisibility[row][col]);
    
    
    //REMOVE TODAS AS OCORRÊNCIAS DA MESMA SINERGIA LIBERANDO ESPAÇO
    for (auto i = 0; i < g_pBM3Macro->MAX_SYNERGY_COUNT; i++)
    {
        //procura por slots com a sequencia desmarcada e a remove
        if (g_pBM3Macro->sComboData.combo[i] == row)
        {
            g_pBM3Macro->sComboData.combo[i] = -1;
            //printf("REMOVIDO DO INDEX %d -> SEQUENCIA %08X\n", i, row);
        }
    }
    
    //INSERÇÃO DOS DADOS DA SEQUENCIA NO BUFFER DE ATAQUES
    if (g_pBM3Macro->sComboData.btnVisibility[row][col])
    {
        int tmpCounter = 0;
        //procura por slots livres e escreve a sequencia
        for (auto i = 0; i < g_pBM3Macro->MAX_SYNERGY_COUNT; i++)
        {
            if (g_pBM3Macro->sComboData.combo[i] == -1 && tmpCounter <= col)
            {
                g_pBM3Macro->sComboData.combo[i] = row;
                //printf("INSERTADO NO INDEX %d -> SEQUENCIA %08X\n", i, row);
                tmpCounter++;
            }
        }
    }
    
    //INCREMENTA A SINERGIA COM A QUANTIDADE ATUAL
    int sinergyInBufferCtn = 0;
    for (int i = 0; i < g_pBM3Macro->MAX_SYNERGY_COUNT; i++)
    {
        if (g_pBM3Macro->sComboData.combo[i] != -1)
        {
            sinergyInBufferCtn++;
        }
    }
    
    //RESETA A DEFINIÇÃO ANTERIOR
    g_pBM3Macro->ResetAttacks();
    
    //SALVA AS SEQUENCIAS ATUAIS
    g_pBM3Macro->sequences = sinergyInBufferCtn;

    //SALVA OS DADOS
    g_pBM3Macro->SaveConfigFile(g_pBM3Macro->macroPath, &g_pBM3Macro->sComboData, sizeof(g_pBM3Macro->sComboData));
    VM_END;
};

void BM3Macro::OnSelectFatalOptions(cSzObjectContext* pObjectCtx, int slot, void*, WINPROC_CLICKIDX clickType)
{
    VM_STARTV(__FUNCTION__);
    //limpa todos os botões
    for (auto& it : g_pBM3Macro->obj_RadioButton_Fatal)
        it.SetSelected(false);
    
    //seleciona o botão atual clicado
    g_pBM3Macro->obj_RadioButton_Fatal[slot-1].SetSelected(true);
    
    //passa o valor selecionado para o fatal count
    g_pBM3Macro->sComboData.fatalCount = slot;
    
    //SALVA OS DADOS
    g_pBM3Macro->SaveConfigFile(g_pBM3Macro->macroPath, &g_pBM3Macro->sComboData, sizeof(g_pBM3Macro->sComboData));
    VM_END;
}

void BM3Macro::NotifyDevice()
{
    VM_STARTU(__FUNCTION__);
    //objects
    obj_MainBoxMaximized.RegisterObject(vObjects)
        ->AddTexture(0, mUI_Texture[1], { 543, 4, 170, 226 }, 0, 0, 0, false, false, false);

    obj_MainBoxMinimized.RegisterObject(vObjects)
        ->AddTexture(0, mUI_Texture[1], { 543, 4, 170, 22 }, 0, 0, 0, false, false, false);

    //others
    obj_MinimizeBtn.RegisterObject(vObjects)
        ->AddTexture(0, mUI_Texture[0], { 698, 66, 13, 13 }, 0, 0, 0, false, false, false)
        ->AddCallback(MinimizeWindow, 0, 0);

    obj_MaximizeBtn.RegisterObject(vObjects)
        ->AddTexture(0, mUI_Texture[0], { 698, 49, 13, 13 }, 0, 0, 0, false, false, false)
        ->AddCallback(MaximizeWindow, 0, 0);

    //window name
    txt_WindowName.RegisterObject(vObjects)
        ->AddText("BM3 Combo", { 0,0,200,30 }, DT_LEFT, 17, color_WhiteSmoke);

    //radio buttons
    for (int i = 0; i < 5; i++)
    {
        obj_AddImages[i].RegisterObject(vObjects)
            ->AddTexture(0, mUI_Texture[4], { 13, 986, 149, 25 }, 0, 0, 0, false, false, false);
    }

    //attack selection
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            auto& objRef = obj_RadioButton[i][j];

            objRef.RegisterObject(vObjects)
                ->AddTexture(eBTN_TYPES::E_BTN_NORMAL, mUI_Texture[0], { 747,680,14,14 }, 0, 0, 0, false, false, false)
                ->AddTexture(eBTN_TYPES::E_BTN_ONRECT, mUI_Texture[0], { 747,666,15,15 }, 0, 0, 0, true, false, true)
                ->AddTexture(eBTN_TYPES::E_BTN_CLICKED, mUI_Texture[0], { 747,666,15,15 }, 0, 0, 0, false, true, true)
                ->AddCallback(OnSelectSynergyOptions, 0, 0);

            objRef.rowIdx = i;
            objRef.colIdx = j;
        }
    }

    //fatal selection
    for (int i = 0; i < 3; i++)
    {
        obj_RadioButton_Fatal[i].RegisterObject(vObjects)
            ->AddTexture(eBTN_TYPES::E_BTN_NORMAL, mUI_Texture[0], { 747,680,14,14 }, 0, 0, 0, false, false, false)
            ->AddTexture(eBTN_TYPES::E_BTN_ONRECT, mUI_Texture[0], { 747,666,15,15 }, 0, 0, 0, true, false, true)
            ->AddTexture(eBTN_TYPES::E_BTN_CLICKED, mUI_Texture[0], { 747,666,15,15 }, 0, 0, 0, false, true, true)
            ->AddCallback(OnSelectFatalOptions, i + 1, 0);
    }

    //sequence name
    txt_SequenceName[0].RegisterObject(vObjects)
        ->AddText("A-A-B-A", { 0,0,200,30 }, DT_LEFT, 17, color_WhiteSmoke);

    txt_SequenceName[1].RegisterObject(vObjects)
        ->AddText("A-B-A-A", { 0,0,200,30 }, DT_LEFT, 17, color_WhiteSmoke);

    txt_SequenceName[2].RegisterObject(vObjects)
        ->AddText("B-A-A-B", { 0,0,200,30 }, DT_LEFT, 17, color_WhiteSmoke);

    txt_SequenceName[3].RegisterObject(vObjects)
        ->AddText("B-B-A-B", { 0,0,200,30 }, DT_LEFT, 17, color_WhiteSmoke);

    //Synergy Index
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            txt_SinergyIndex[i][j].RegisterObject(vObjects)
                ->AddText(std::format("{}x", j+1).c_str(), {0,0,200,30}, DT_LEFT, 13, color_WhiteSmoke);
        }
    }

    //fatal index
    txt_FatalIndex[0].RegisterObject(vObjects)
        ->AddText("Fatal I", { 0,0,200,30 }, DT_LEFT, 13, color_WhiteSmoke);

    txt_FatalIndex[1].RegisterObject(vObjects)
        ->AddText("Fatal II", { 0,0,200,30 }, DT_LEFT, 13, color_WhiteSmoke);

    txt_FatalIndex[2].RegisterObject(vObjects)
        ->AddText("Fatal III", { 0,0,200,30 }, DT_LEFT, 13, color_WhiteSmoke);

    //seta a visibilidade padrão
    obj_MainBoxMinimized.SetVisible(true);
    obj_MaximizeBtn.SetVisible(true);
    txt_WindowName.SetVisible(true);
    VM_END;
}

void BM3Macro::Present()
{
    //sincroniza a barra principal com a minimizada
    obj_MainBoxMinimized.SetMove();
    auto& coordBase             = obj_MainBoxMinimized.objRect;

    //salva a posição
    g_pBM3Macro->sComboData.uiPosition = { coordBase.left,coordBase.top };

    obj_MainBoxMaximized.objRect = coordBase;

    //ajusta a posição dos clicáveis com base na box principal
    obj_MinimizeBtn.SetPositionL(coordBase.left + 147, coordBase.top + 5);
    obj_MaximizeBtn.SetPositionL(coordBase.left + 147, coordBase.top + 5);

    //boxes
    obj_AddImages[0].SetPositionL(coordBase.left + 11, coordBase.top + 33);
    obj_AddImages[1].SetPositionL(coordBase.left + 11, coordBase.top + 71);
    obj_AddImages[2].SetPositionL(coordBase.left + 11, coordBase.top + 109);
    obj_AddImages[3].SetPositionL(coordBase.left + 11, coordBase.top + 147);
    obj_AddImages[4].SetPositionL(coordBase.left + 11, coordBase.top + 189);
    
    //window name
    txt_WindowName.SetPositionL(coordBase.left + 43, coordBase.top + 4);
    
    //descs
    txt_Descriptions[0].SetPositionL(coordBase.left + 16, coordBase.top + 94);
    txt_Descriptions[1].SetPositionL(coordBase.left + 16, coordBase.top + 164);
    txt_Descriptions[2].SetPositionL(coordBase.left + 16, coordBase.top + 234);
    txt_Descriptions[3].SetPositionL(coordBase.left + 16, coordBase.top + 304);
    
    //sequences
    txt_SequenceName[0].SetPositionL(coordBase.left + 55, coordBase.top + 35);
    txt_SequenceName[1].SetPositionL(coordBase.left + 55, coordBase.top + 73);
    txt_SequenceName[2].SetPositionL(coordBase.left + 55, coordBase.top + 111);
    txt_SequenceName[3].SetPositionL(coordBase.left + 55, coordBase.top + 149);
    
    //radio buttons selection sinergy
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            obj_RadioButton[i][j].SetPositionL(coordBase.left + 30 + (j * 50), (coordBase.top + 58) + (38 * i));
        }
    }
    
    //radio button fatais
    for (int i = 0; i < 3; i++)
    {
        obj_RadioButton_Fatal[i].SetPositionL(coordBase.left + 46 + (i * 50), coordBase.top + 195);
    }
    
    //textos
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            txt_SinergyIndex[i][j].SetPositionL(coordBase.left + 16 + (j * 50), (coordBase.top + 57) + (38 * i));
        }
    }
    
    //index name of fatal bm3
    txt_FatalIndex[0].SetPositionL(coordBase.left + 14, coordBase.top + 195);
    txt_FatalIndex[1].SetPositionL(coordBase.left + 62, coordBase.top + 195);
    txt_FatalIndex[2].SetPositionL(coordBase.left + 110, coordBase.top + 195);
}

void BM3Macro::Draw()
{
    auto DrawTextures = [](cSzObjectContext* pObj)
        {
            D3DXVECTOR3 texRect;
            D3DXMATRIX currentMatrix, oldMatrix;

            if (!pObj->IsVisible())
                return;

            for (auto& textureList : pObj->m_textureData)
            {
                auto& textureData = textureList.second;

                //calculo dos objetos validos
                RECT objectRect =
                {
                    static_cast<int>((pObj->objRect.left + textureData.addXPos) * textureData.GetScale()),
                    static_cast<int>((pObj->objRect.top + textureData.addYPos) * textureData.GetScale()),
                    static_cast<int>((pObj->objRect.left + pObj->objRect.right + textureData.addXPos) * textureData.GetScale()),
                    static_cast<int>((pObj->objRect.top + pObj->objRect.bottom + textureData.addYPos) * textureData.GetScale())
                };

                //se a textura for inválida
                if (!textureData.pTexture)
                    continue;

                //se tiver eventos
                if (textureData.useEventMouse)
                {
                    //se não estiver clicado e for da opção de ver ao clicar
                    if (!pObj->IsSelected() && textureData.viewOnClick)
                        continue;

                    //se não estiver no retângulo e for do tipo de hover
                    if (!PtInRect(&objectRect, cInputs->mousePos) && textureData.isHover)
                        continue;
                }

                //calculo dos textos validos
                texRect =
                {
                    static_cast<float>(pObj->objRect.left + textureData.addXPos) ,
                    static_cast<float>(pObj->objRect.top + textureData.addYPos), 0.0f
                };

                DirectGlobalSprite->GetTransform(&oldMatrix);
                DirectGlobalSprite->SetTransform(D3DXMatrixScaling(&currentMatrix, textureData.GetScale(), textureData.GetScale(), 0));
                DirectGlobalSprite->Draw(textureData.pTexture, &textureData.sourceRect, NULL, &texRect, -1);
                DirectGlobalSprite->SetTransform(&oldMatrix);
            }

            if (auto pFont = cInputs->GetFontBySize(static_cast<int>(pObj->sFont.fontSize * ObjectScaleX)))
            {
                auto sFontData = &pObj->sFont;

                if (!sFontData->calculated)
                    pFont->DrawTextA(NULL, sFontData->objText.c_str(), -1, &sFontData->fontRectangle, DT_CALCRECT, -1);

                RECT fontRect =
                {
                    static_cast<int>((pObj->objRect.left + sFontData->fontPosition.x) * ObjectScaleX),
                    static_cast<int>((pObj->objRect.top + sFontData->fontPosition.y) * ObjectScaleY),
                     sFontData->removeDelimitX ? MaxResolutionX : static_cast<int>((pObj->objRect.left + pObj->objRect.right + sFontData->fontPosition.x) * ObjectScaleX),
                     sFontData->removeDelimitY ? MaxResolutionY : static_cast<int>((pObj->objRect.top + pObj->objRect.bottom + sFontData->fontPosition.y) * MaxResolutionY)
                };

                pFont->DrawTextA(DirectGlobalSprite, sFontData->objText.c_str(), -1, &fontRect, sFontData->iDT_Format, pObj->colorRGBA);
            }
        };


    if (!MyUserCtx->isLoggedChar)
        ObjVisible = false;

    if (!ObjVisible)
        return;

    DirectGlobalSprite->Begin(D3DXSPRITE_ALPHABLEND);

    for (auto& it : vObjects)
    {
        if (!it->IsVisible())
            continue;

        DrawTextures(it);
    }

    DirectGlobalSprite->End();

    //BM3
    AttackManager();
}

void BM3Macro::Release()
{
    DirectGlobalSprite->OnLostDevice();
    DirectGlobalSprite->OnResetDevice();
}

LRESULT CALLBACK BM3Macro::OnKeyBoardEvent(WPARAM wParam, WPARAM wParamTranslated)
{
    return TRUE;
}

LRESULT CALLBACK BM3Macro::MouseClick(WINPROC_CLICKIDX state)
{
    LRESULT result = TRUE;

    bool callbackExecuted   = false;

    auto SetPointClick = [&](cSzObjectContext* pObj, bool keydown, bool& callbackExecuted)
        {
            if (pObj->IsVisible() && pObj->IsInRect(cInputs->mousePos.x, cInputs->mousePos.y) && keydown)
            {
                pObj->clicked = keydown;

                //pega a diferença do clique para o left e top do objRect para obter a distancia do clique e armazenar
                pObj->pointClick.x = (int)(cInputs->mousePos.x / ObjectScaleX) - pObj->objRect.left;
                pObj->pointClick.y = (int)(cInputs->mousePos.y / ObjectScaleY) - pObj->objRect.top;

                if (!callbackExecuted && pObj->vCallback.size())
                {
                    pObj->isSelectedObject = true;

                    result = false;

                    pObj->RunCallbacks(state);

                    callbackExecuted = true;

                    return;
                }
            }
            else
            {
                pObj->clicked = false;
            }
        };

    for (auto& it : vObjects)
    {
        switch (state)
        {
        case WP_LEFT_CLICKDOWN:
        case WP_SHIFT_LEFT_CLICKDOWN:
            SetPointClick(it, true, callbackExecuted);
            break;
        case WP_CLICKUP:
        case WP_SHIFT_CLICKUP:
        case WP_CONTROL_CLICKUP:
            SetPointClick(it, false, callbackExecuted);
            break;
        default:
            break;
        }
    }
    return result;
}

LRESULT CALLBACK BM3Macro::MouseMove(WPARAM wParam)
{
    LRESULT result = TRUE;

    for (auto& it : vObjects)
    {
        //se está visivel e a condição é igual
        if (it->moveOnClick && it->clicked && wParam == WINPROC_CLICKIDX::WP_LEFT_CLICKDOWN && it->IsVisible())
        {
            //X representa a diferença entre a área clicada da área inicial do retangulo
            it->objRect.left = ((int)(cInputs->mousePos.x / ObjectScaleX) - it->pointClick.x);
            it->objRect.top = ((int)(cInputs->mousePos.y / ObjectScaleY) - it->pointClick.y);

            //checa se as coordenadas estão negativas e seta para neutras (0)
            if (it->objRect.left < 0) { it->objRect.left = 0; };
            if (it->objRect.top < 0) { it->objRect.top = 0; };

            //posição X do cursor + o tamanho do objeto menos o ponto em que foi clicado
            auto endPosX = (it->objRect.left + it->objRect.right);
            if (endPosX > (MaxResolutionX / ObjectScaleX))
                it->objRect.left = (int)((MaxResolutionX / ObjectScaleX) - it->objRect.right);

            //posição Y do cursor + o tamanho do objeto menos o ponto em que foi clicado
            auto endPosY = (it->objRect.top + it->objRect.bottom);
            if (endPosY > (MaxResolutionY / ObjectScaleY))
                it->objRect.top = (int)((MaxResolutionY / ObjectScaleY) - it->objRect.bottom);
            
            //printf("Left Coord X %d - Left Coord Y %d  |  Right Coord X %f - Right Coord Y %f\n", it->objRect.left, it->objRect.top, (MaxResolutionX / ObjectScaleX) - it->objRect.left, (MaxResolutionY / ObjectScaleY) - it->objRect.top);
            result = FALSE;
            break;
        }

    }
    return result;
}

void BM3Macro::SaveOnClose()
{
    //SALVA OS DADOS
    g_pBM3Macro->SaveConfigFile(macroPath, &g_pBM3Macro->sComboData, sizeof(g_pBM3Macro->sComboData));
}

void BM3Macro::ResizeWindow()
{
    for (auto& it : vObjects)
    {
        it->ResizeWindow();
    }
}

void BM3Macro::LoadJson()
{
    VM_STARTV(__FUNCTION__);
    std::ifstream f(std::format("{}\\Data\\UserData\\BM3Macro\\BM3Macro.json", Basic::Game::GetGameDir()));
    if (f.is_open())
    {
        nlohmann::json data = nlohmann::json::parse(f);

        for (const auto& macro : data["Bm3Macro"])
        {
            int classe = macro["ClassId"];
            auto& mapRef = m_BM3Data[classe];
            int sequenceIndex = 0;

            for (const auto& sequences : macro["Sequences"])
            {
                std::string result = sequences;

                for (int i = 0; i < 4; i++)
                {
                    if (i > result.size())
                        break;

                    mapRef.sinergyData[sequenceIndex].Attack[i] = std::tolower(result.at(i)) == 'a' ? 0 : 1;
                }

                sequenceIndex++;
            }
        }
    }
    VM_END;
}

void BM3Macro::Init()
{
    Memory::WriteValue(0x009986A0, InitializeStyleEX);
    Memory::WriteValue(0x00988C75, NfySkillToUser);
    LoadJson();
    RegisterWindow(WINDOW_NAME_IDX::UI_BM3_MACRO, false, g_pBM3Macro.get(), eVisibleType::SCREEN_FRONT, eCondition::INGAME, ::UNKNOWN_LASTUI);
    REGISTERCUSTOMPROC(MAINCMD_VALUE_CUSTOM::S2C_BM3MACRORESULT, OnS2CBM3MacroResponse);
}
