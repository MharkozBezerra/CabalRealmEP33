#include "Objects.h"
#include <format>
#include <filesystem>
#include "../../DirectX/DirectX.h"
#include "../../Basic/Basic.h"

//instancia do objeto que é usado como extern
std::vector<S_DrawBase> v_BaseObjects;

//endereços das texturas carregadas por numero
std::unordered_map<int, cSzObjectContext> mUI_Texture;

cSzInputData* cInputs = new(cSzInputData);


void cSzObjectContext::SetTitleCabalHint(std::string title)
{
    RECT tmpPos = { 0,0,0,0 };

    sTextBox.title = title;
    if (auto pFont = cInputs->GetFontBySize((int)(sFont.fontSize * GetScaleX())))
    {
        /*******************************************TITLE***************************************************/
        pFont->DrawTextA(DirectGlobalSprite, sTextBox.title.c_str(), -1, &tmpPos, DT_CALCRECT, NULL);
        if (objRect.right < tmpPos.right)
            objRect.right = tmpPos.right;

        if (objRect.bottom < tmpPos.bottom)
            objRect.bottom = tmpPos.bottom;
    }
}

void cSzObjectContext::SetTextCabalHint(std::string text)
{
    RECT tmpPos = { 0,0,0,0 };

    sTextBox.text = text;
    if (auto pFont = cInputs->GetFontBySize((int)(sFont.fontSize * GetScaleX())))
    {
        /*******************************************MENSAGEM***************************************************/
        pFont->DrawTextA(DirectGlobalSprite, sTextBox.text.c_str(), -1, &tmpPos, DT_CALCRECT | DT_WORDBREAK, NULL);

        if (objRect.right < tmpPos.right)
            objRect.right = tmpPos.right + 19;

        if (objRect.bottom < tmpPos.bottom)
            objRect.bottom = tmpPos.bottom + 42;
    }
}

void cSzObjectContext::CreateCabalHint(std::vector<cSzObjectContext*>& pVec, eOBJTYPE objType, int x, int y, int w, int h, std::string title, std::string text, cSzObjectContext* image, int fontSize)
{

    bIsHint = true;
    ObjType = objType;
    objRect = { x,y,w,h };
    sTextBox.title = title;
    sTextBox.text = text;
    sTextBox.img = image;
    sFont.fontSize = fontSize;
    //g_MessageMgr->GenerateCabalString(&hintText, sTextBox.title.c_str());
    pVec.push_back(this);
}

void cSzObjectContext::CreateCabalHint(eOBJTYPE objType, int x, int y, int w, int h, std::string title, std::string text, cSzObjectContext* image, int fontSize)
{

    bIsHint = true;
    ObjType = objType;
    objRect = { x,y,w,h };
    sTextBox.title = title;
    sTextBox.text = text;
    sTextBox.img = image;
    sFont.fontSize = fontSize;
    //g_MessageMgr->GenerateCabalString(&hintText, sTextBox.title.c_str());
}

float cSzObjectContext::GetScaleX()
{
    return ObjectScaleX + ((ObjectScaleX / 100) * fProportionPercent);
}

float cSzObjectContext::GetScaleY()
{
    return ObjectScaleY + ((ObjectScaleY / 100) * fProportionPercent);
}

cSzObjectContext* cSzObjectContext::SetProportion(float percent)
{
    fProportionPercent = percent;
    return this;
}

void cSzObjectContext::CabalHint()
{
    DWORD oldFVF;
    //salva o estado atual
    Direct3DDevice->GetFVF(&oldFVF);

    RECT rc = { 0,0,0,0 };
    RECT title = { 0,0,0,0 };
    RECT text = { 0,0,0,0 };
    D3DRECT left, top, right, bottom, separator;

    if (ObjType == NORMAL_HINT)
    {
        if (auto pFont = cInputs->GetFontBySize((int)(sFont.fontSize * (GetScaleX()))))
        {
            /*******************************************TITLE***************************************************/
            pFont->DrawTextA(NULL, sTextBox.title.c_str(), -1, &title, DT_CALCRECT, NULL);

            /*******************************************MENSAGEM***************************************************/
            pFont->DrawTextA(NULL, sTextBox.text.c_str(), -1, &text, DT_CALCRECT | DT_WORDBREAK, NULL);

            objRect.right = title.right > text.right ? title.right + 20 : text.right + 20;
            objRect.bottom = title.bottom > text.bottom ? title.bottom + 44 : text.bottom + 44;
        }

        //BOX
        int x = (int)(objRect.left * (GetScaleX()));
        int y = (int)(objRect.top * (GetScaleY()));
        int w = (int)(objRect.right);
        int h = (int)(objRect.bottom);
        DrawBox((float)x, (float)y, (float)w, (float)h, D3DCOLOR_RGBA(0, 0, 0, 190));

        //SEPARATOR
        separator.x1 = x + 4;
        separator.x2 = x + w - 4;
        separator.y1 = y + title.bottom + 10;
        separator.y2 = y + title.bottom + 11;
        Direct3DDevice->Clear(1, &separator, D3DCLEAR_TARGET, D3DCOLOR_RGBA(117, 119, 119, 255), 0, 0);

        left.x1 = x;
        left.x2 = x + 1;
        left.y1 = y;
        left.y2 = y + h - 1;
        Direct3DDevice->Clear(1, &left, D3DCLEAR_TARGET, D3DCOLOR_RGBA(113, 113, 116, 255), 0, 0);

        top.x1 = x;
        top.x2 = x + w - 1;
        top.y1 = y;
        top.y2 = y + 1;
        Direct3DDevice->Clear(1, &top, D3DCLEAR_TARGET, D3DCOLOR_RGBA(117, 119, 119, 255), 0, 0);

        right.x1 = x + w - 1;
        right.x2 = x + w;
        right.y1 = y;
        right.y2 = y + h;
        Direct3DDevice->Clear(1, &right, D3DCLEAR_TARGET, D3DCOLOR_RGBA(51, 51, 52, 255), 0, 0);

        bottom.x1 = x;
        bottom.x2 = x + w;
        bottom.y1 = y + h - 1;
        bottom.y2 = y + h;
        Direct3DDevice->Clear(1, &bottom, D3DCLEAR_TARGET, D3DCOLOR_RGBA(117, 119, 119, 255), 0, 0);

        if (auto pFont = cInputs->GetFontBySize((int)(sFont.fontSize * GetScaleX())))
        {
            /*******************************************MENSAGEM***************************************************/
            SetRect(&rc, x + 6, y + title.bottom + 15, x + w + 14, y + h + title.bottom + 15);
            pFont->DrawTextA(NULL, sTextBox.text.c_str(), -1, &rc, DT_LEFT | DT_WORDBREAK, color_WhiteSmoke);

            /*******************************************DESCRIÇÃO***************************************************/
            SetRect(&rc, x, y + 6, x + w, y + h);
            pFont->DrawTextA(NULL, this->sTextBox.title.c_str(), -1, &rc, DT_CENTER | DT_WORDBREAK, D3DCOLOR_RGBA(0, 242, 145, 255));
        }
    }
    if (ObjType == SIMPLE_HINT)
    {
        if (auto pFont = cInputs->GetFontBySize((int)(sFont.fontSize * (GetScaleX()))))
        {
            /*******************************************TITLE***************************************************/
            pFont->DrawTextA(NULL, sTextBox.title.c_str(), -1, &title, DT_CALCRECT, NULL);

            objRect.right = title.right + 20;
            objRect.bottom = title.bottom + 10;
        }

        //BOX
        int x = (int)(objRect.left * (GetScaleX()));
        int y = (int)(objRect.top * (GetScaleY()));
        int w = (int)(objRect.right);
        int h = (int)(objRect.bottom);
        DrawBox((float)x, (float)y, (float)w, (float)h, D3DCOLOR_RGBA(0, 0, 0, 190));

        left.x1 = x;
        left.x2 = x + 1;
        left.y1 = y;
        left.y2 = y + h - 1;
        Direct3DDevice->Clear(1, &left, D3DCLEAR_TARGET, D3DCOLOR_RGBA(113, 113, 116, 255), 0, 0);

        top.x1 = x;
        top.x2 = x + w - 1;
        top.y1 = y;
        top.y2 = y + 1;
        Direct3DDevice->Clear(1, &top, D3DCLEAR_TARGET, D3DCOLOR_RGBA(117, 119, 119, 255), 0, 0);

        right.x1 = x + w - 1;
        right.x2 = x + w;
        right.y1 = y;
        right.y2 = y + h;
        Direct3DDevice->Clear(1, &right, D3DCLEAR_TARGET, D3DCOLOR_RGBA(51, 51, 52, 255), 0, 0);

        bottom.x1 = x;
        bottom.x2 = x + w;
        bottom.y1 = y + h - 1;
        bottom.y2 = y + h;
        Direct3DDevice->Clear(1, &bottom, D3DCLEAR_TARGET, D3DCOLOR_RGBA(117, 119, 119, 255), 0, 0);

        if (auto pFont = cInputs->GetFontBySize((int)(sFont.fontSize * GetScaleX())))
        {
            SetRect(&rc, x + 3, y + 5, x + w, y + h);
            pFont->DrawTextA(NULL, this->sTextBox.title.c_str(), -1, &rc, DT_CENTER | DT_WORDBREAK, color_WhiteSmoke);
        }
    }

    objRect.right = 0;
    objRect.bottom = 0;

    Direct3DDevice->SetFVF(oldFVF);
}

void cSzObjectContext::DrawBoxLine(int x, int y, int w, int h, int lineWeight, D3DCOLOR Color)
{
    D3DRECT left, top, right, bottom;

    left.x1 = x;
    left.x2 = x + lineWeight;
    left.y1 = y;
    left.y2 = y + h;
    Direct3DDevice->Clear(1, &left, D3DCLEAR_TARGET, Color, 0, 0);

    top.x1 = x;
    top.x2 = x + w;
    top.y1 = y;
    top.y2 = y + lineWeight;
    Direct3DDevice->Clear(1, &top, D3DCLEAR_TARGET, Color, 0, 0);

    right.x1 = x + w - lineWeight;
    right.x2 = x + w;
    right.y1 = y;
    right.y2 = y + h;
    Direct3DDevice->Clear(1, &right, D3DCLEAR_TARGET, Color, 0, 0);

    bottom.x1 = x;
    bottom.x2 = x + w;
    bottom.y1 = y + h - lineWeight;
    bottom.y2 = y + h;
    Direct3DDevice->Clear(1, &bottom, D3DCLEAR_TARGET, Color, 0, 0);
}

void cSzObjectContext::DrawBox(float x, float y, float w, float h, D3DCOLOR Color)
{
    struct Vertex
    {
        float x, y, z, ht;
        DWORD Color;
    }
    V[4] = {
        { x, y + h, 0.0f, 0.0f, Color },
        { x, y, 0.0f, 0.01f, Color },
        { x + w, y + h, 0.0f, 0.0f, Color },
        { x + w, y, 0.0f, 0.0f, Color }
    };

    Direct3DDevice->SetTexture(0, NULL);
    Direct3DDevice->SetPixelShader(0);
    Direct3DDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
    Direct3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
    Direct3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, V, sizeof(Vertex));

}

void cSzObjectContext::DrawBox(RECT* rc, D3DCOLOR Color)
{
    DrawBox(static_cast<int>(rc->left * GetScaleX()), static_cast<int>(rc->top * GetScaleX()), static_cast<int>(rc->right * GetScaleX()), static_cast<int>(rc->bottom * GetScaleX()), Color);
}

cSzObjectContext* cSzObjectContext::AddTexture(int index, cSzObjectContext& pObjectRef, RECT sourceRect, int addXPos, int addYPos, float scale, bool isHover, bool viewOnClick, bool useMouseEvents, bool visible)
{
    if (pObjectRef.pTexture)
    {
        RECT tmpRect = { sourceRect.left , sourceRect.top ,sourceRect.left + sourceRect.right ,  sourceRect.top + sourceRect.bottom };

        this->m_textureData[index] = { pObjectRef.pTexture ,tmpRect, addXPos,addYPos ,scale ,isHover,viewOnClick ,useMouseEvents, visible };

        this->objRect.right     = sourceRect.right;
        this->objRect.bottom    = sourceRect.bottom;
    }
    return this;
}

cSzObjectContext* cSzObjectContext::AddTexture(int index, LPDIRECT3DTEXTURE9 pTexture, RECT sourceRect, int addXPos, int addYPos, float scale, bool isHover, bool viewOnClick, bool useMouseEvents, bool visible)
{
    RECT tmpRect = { sourceRect.left , sourceRect.top ,sourceRect.left + sourceRect.right ,  sourceRect.top + sourceRect.bottom };

    this->m_textureData[index] = { pTexture ,tmpRect , addXPos,addYPos, scale, isHover,viewOnClick ,useMouseEvents,visible };

    this->objRect.right = sourceRect.right;
    this->objRect.bottom = sourceRect.bottom;

    return this;
}

cSzObjectContext* cSzObjectContext::AddTexture(int index, const char* pTextureFolder, RECT sourceRect, int addXPos, int addYPos, float scale, bool isHover, bool viewOnClick, bool useMouseEvents, bool visible)
{
    LPDIRECT3DTEXTURE9 tmpTexture;
    RECT tmpRect = { sourceRect.left , sourceRect.top ,sourceRect.left + sourceRect.right ,  sourceRect.top + sourceRect.bottom, };

    if (!SUCCEEDED(D3DXCreateTextureFromFileA(Direct3DDevice, pTextureFolder, &tmpTexture)))
    {
        printf("{} nao foi carregado\n", pTextureFolder);
    }
    else
    {
        this->m_textureData[index] = { tmpTexture ,tmpRect , addXPos, addYPos ,scale,isHover,viewOnClick ,useMouseEvents , visible};
        this->objRect.right     = sourceRect.right ;
        this->objRect.bottom    = sourceRect.bottom;
    }

    return this;
}

cSzObjectContext* cSzObjectContext::EraseTexture(int index)
{
    if (this->m_textureData.contains(index))
    {
        auto& ref = this->m_textureData[index];
       
        if (ref.pTexture)
            ref.pTexture->Release();

        this->m_textureData.erase(index);
    }
    return this;
}

void cSzObjectContext::DrawBox(D3DCOLOR Color)
{
    DrawBox(static_cast<int>(objRect.left * GetScaleX()), static_cast<int>(objRect.top * GetScaleX()), static_cast<int>(objRect.right * GetScaleX()), static_cast<int>(objRect.bottom * GetScaleX()), Color);
}

void cSzObjectContext::DrawBorderBox(int x, int y, int w, int h, D3DCOLOR lineColor, D3DCOLOR boxColor, int lineWeight)
{
    DrawBox((float)x, (float)y, (float)w, (float)h, boxColor);
    DrawBoxLine(x, y, w, h, lineWeight, lineColor);
}

void cSzObjectContext::DrawButton()
{
    D3DXVECTOR3 vec = { (float)objRect.left, (float)objRect.top, 0.0f };

    if (ObjType == GENERIC_BUTTON_SAVESTATE)
    {
        if (IsInRect() && !IsSelected())
            sourceRect = buttonRect[1];//hover
        else if (IsSelected())
            sourceRect = buttonRect[2];//selecionado
        else
            sourceRect = buttonRect[0];//normal
    }
    else if (ObjType == GENERIC_BUTTON_NOSAVE)
    {
        if (IsInRect())
        {
            if (clicked)
                sourceRect = buttonRect[2];//selecionado
            else
                sourceRect = buttonRect[1];//hover
        }
        else
            sourceRect = buttonRect[0];//normal
    }
    else
    {
        return;
    }

    DirectGlobalSprite->Draw(pTexture, &sourceRect, NULL, &vec, colorRGBA);
}

cSzObjectContext* cSzObjectContext::SetAlpha(unsigned char alpha)
{
    colorRGBA = D3DCOLOR_RGBA(255, 255, 255, alpha);
    return this;
}

cSzObjectContext* cSzObjectContext::SetPositionL(int x, int y)
{
    objRect.left = x;
    objRect.top = y;

    SetPositionExtraObj_L(x, y);
    return this;
}

cSzObjectContext* cSzObjectContext::SetWindowCenter(int x, int y)
{
    // Calcula o centro da janela levando em consideração o fator de escala
    objRect.left = (int)((float)MaxResolutionX / (2.0f * GetScaleX())) - (objRect.right / 2);
    objRect.top = (int)((float)MaxResolutionY / (2.0f * GetScaleY())) - (objRect.bottom / 2);
    return this;
}

cSzObjectContext* cSzObjectContext::SetPositionExtraObj_L(int x, int y)
{
    if (this->extraObjPtr)
    {
        extraObjPtr->objRect.left = x;
        extraObjPtr->objRect.top = y;
    }
    return this;
}

cSzObjectContext* cSzObjectContext::SetPositionCenterW(int x, int y)
{
    objRect.left = (int)(((MaxResolutionX / GetScaleX()) - ((MaxResolutionX / 2) / GetScaleX())) - x);
    objRect.top = (int)((MaxResolutionY / GetScaleY()) - y);
    return this;
}

cSzObjectContext* cSzObjectContext::SetPositionCenterH(int x, int y)
{
    objRect.left = (int)((MaxResolutionX / GetScaleX()) - x);
    objRect.top = (int)(((MaxResolutionY / GetScaleY()) - ((MaxResolutionY / 2) / GetScaleY())) - y);
    return this;
}

cSzObjectContext* cSzObjectContext::SetPositionR(int x, int y)
{
    objRect.left = (int)(MaxResolutionX / GetScaleX()) - x;
    objRect.top = (int)(MaxResolutionY / GetScaleY()) - y;

    SetPositionExtraObj_R(x, y);
    return this;
}

cSzObjectContext* cSzObjectContext::SetPositionRT(int x, int y)
{
    objRect.left = (int)(MaxResolutionX / GetScaleX()) - objRect.right - x;
    objRect.top = y;
    return this;
}

cSzObjectContext* cSzObjectContext::SetPositionExtraObj_R(int x, int y)
{
    if (this->extraObjPtr)
    {
        extraObjPtr->objRect.left = (int)(MaxResolutionX / GetScaleX()) - x;
        extraObjPtr->objRect.top = (int)(MaxResolutionY / GetScaleY()) - y;
    }
    return this;
}

cSzObjectContext* cSzObjectContext::SetVisible(bool status)
{
    bIsVisible = status;
    return this;
}

cSzObjectContext* cSzObjectContext::SetSize(int w, int h)
{
    objRect.right = w;
    objRect.bottom = h;
    return this;
}

cSzObjectContext* cSzObjectContext::SetW(int w)
{
    objRect.right = w;
    return this;
}

cSzObjectContext* cSzObjectContext::SetH(int h)
{
    objRect.bottom = h;
    return this;
}

cSzObjectContext* cSzObjectContext::SetSourceW(int w)
{
    sourceRect.right = w;
    return this;
}

cSzObjectContext* cSzObjectContext::SetSourceH(int h)
{
    sourceRect.bottom = h;
    return this;
}

cSzObjectContext* cSzObjectContext::SetMove()
{
    moveOnClick = true;
    return this;
};

cSzObjectContext* cSzObjectContext::CreateText(std::vector<cSzObjectContext*>& pVec, std::string text, int pDT_Format, int fontSize, D3DXCOLOR color, int pAddPosX, int pAddPosY, bool useDtCalcRect, int x, int y)
{
    sFont.iDT_Format = pDT_Format;
    sFont.objText = text;
    sFont.fontSize = fontSize;
    sFont.fontPosition = { pAddPosX , pAddPosY };
    colorRGBA = color;
    sFont.isOnlyFont = true;
    objRect = { x , y , 0, 0 };

    if (useDtCalcRect)
        SetText(sFont.objText);

    pVec.push_back(this);
    return this;
};

cSzObjectContext* cSzObjectContext::CreateText(std::string text, int pDT_Format, int fontSize, D3DXCOLOR color, RECT textRect, int pAddPosX, int pAddPosY, bool useDtCalcRect)
{
    sFont.iDT_Format = pDT_Format;
    sFont.objText = text;
    sFont.fontSize = fontSize;
    sFont.fontPosition = { pAddPosX , pAddPosY };
    colorRGBA = color;
    sFont.isOnlyFont = true;
    objRect = { textRect };

    if (useDtCalcRect)
        SetText(sFont.objText);

    return this;
};

cSzObjectContext* cSzObjectContext::AddText(int order, std::string text, RECT textRect, D3DXCOLOR color, int pAddPosX, int pAddPosY, CabalTextAlignment fontPos, bool useShadow)
{
    auto textData           = &m_fontData[order];
    textData->objText       = text;
    textData->fontPosition  = { pAddPosX ,pAddPosY };
    textData->fontPos       = fontPos;
    textData->useShadow     = useShadow;
    textData->colorRGBA     = color;
    objRect                 = { textRect };
    sourceRect              = { textRect };

    return this;
};

cSzObjectContext* cSzObjectContext::AddText(int order, std::string text, D3DXCOLOR color, int pAddPosX, int pAddPosY, CabalTextAlignment fontPos, bool useShadow)
{
    auto textData = &m_fontData[order];
    textData->objText = text;
    textData->fontPosition = { pAddPosX ,pAddPosY };
    textData->fontPos = fontPos;
    textData->useShadow = useShadow;
    textData->colorRGBA = color;

    return this;
};


cSzObjectContext* cSzObjectContext::AddText(std::string text, int pDT_Format, int fontSize, D3DXCOLOR color, int pAddPosX, int pAddPosY)
{
    sFont.iDT_Format    = pDT_Format;
    sFont.objText       = text;
    sFont.fontSize      = fontSize;
    sFont.fontPosition  = { pAddPosX ,pAddPosY };
    colorRGBA           = color; 
    return this;
};

cSzObjectContext* cSzObjectContext::AddText(std::string text, RECT textRect, int pDT_Format, int fontSize, D3DXCOLOR color, int pAddPosX, int pAddPosY)
{
    sFont.iDT_Format    = pDT_Format;
    sFont.objText       = text;
    sFont.fontSize      = fontSize;
    sFont.fontPosition  = { pAddPosX ,pAddPosY };
    colorRGBA           = color;
    objRect             = { textRect };
    return this;
};

cSzObjectContext* cSzObjectContext::AddText(std::string text, RECT textRect, bool removeDelimitX, bool removeDelimitY, int pDT_Format, int fontSize, D3DXCOLOR color, int pAddPosX, int pAddPosY)
{
    sFont.iDT_Format            = pDT_Format;
    sFont.objText               = text;
    sFont.fontSize              = fontSize;
    sFont.fontPosition          = { pAddPosX ,pAddPosY };
    sFont.objText               = text;
    sFont.removeDelimitX        = removeDelimitX;
    sFont.removeDelimitY        = removeDelimitY;
    colorRGBA                   = color;

    objRect = { textRect };
    return this;
};

cSzObjectContext* cSzObjectContext::DrawString()
{

    RECT rc;

    if (sFont.objText.c_str())
    {
        if (auto pFont = cInputs->GetFontBySize((int)(sFont.fontSize * (GetScaleX()))))
        {
            rc.left = (int)((objRect.left + sFont.fontPosition.x) * (GetScaleX()));
            rc.top = (int)((objRect.top + sFont.fontPosition.y) * (GetScaleY()));

            if (sFont.isOnlyFont)
            {
                rc.right = (int)(MaxResolutionX);
                rc.bottom = (int)(MaxResolutionY);

            }
            else
            {
                //usado para quando o texto for parte de um sprite que precisa ser centralizado no tamanho exato do objeto
                rc.right = (int)((objRect.left + objRect.right + sFont.fontPosition.x) * (GetScaleX()));
                rc.bottom = (int)((objRect.top + objRect.bottom + sFont.fontPosition.y) * (GetScaleY()));
            }
            pFont->DrawTextA(NULL, sFont.objText.c_str(), -1, &rc, sFont.iDT_Format, colorRGBA);
        }
    }
    return this;
}

cSzObjectContext* cSzObjectContext::SetText(std::string text)
{
    RECT rc = { 0,0,0,0 };
    if (auto pFont = cInputs->GetFontBySize(sFont.fontSize))
    {
        pFont->DrawTextA(NULL, text.c_str(), -1, &rc, DT_CALCRECT, 0xFFFFFFFF);

        if (sFont.isOnlyFont)
        {
            objRect.right = rc.right;
            objRect.bottom = rc.bottom;
        }
        sFont.objText = text;
    }
    return this;
}

void cSzObjectContext::SetSelected(bool status)
{
    isSelectedObject = status;
}

cSzObjectContext* cSzObjectContext::SetInputText(std::string text)
{
    RECT rc = { 0,0,0,0 };
    if (auto pFont = cInputs->GetFontBySize(sFont.fontSize))
    {
        pFont->DrawTextA(NULL, text.c_str(), -1, &rc, DT_CALCRECT, 0xFFFFFFFF);
    }
    return this;
}

cSzObjectContext* cSzObjectContext::OnSwitchClickInObject(void* referenceOfVarInClass)
{
    auto Ptr = (cSzObjectContext*)referenceOfVarInClass;

    if (*(DWORD*)Ptr == 0 && this->extraObjPtr)
    {

        *(DWORD*)Ptr = (DWORD)this->extraObjPtr;

        cSzObjectContext* value = (cSzObjectContext*)*(DWORD*)Ptr;

        value->SetSelected(true);
    }
    if (*(DWORD*)Ptr && this->extraObjPtr)
    {
        cSzObjectContext* value = (cSzObjectContext*)*(DWORD*)Ptr;

        value->SetSelected(false);

        *(DWORD*)Ptr = (DWORD)this->extraObjPtr;

        cSzObjectContext* value2 = (cSzObjectContext*)*(DWORD*)Ptr;

        value2->SetSelected(true);
    }
    return this;
}

//GETTERS
D3DXVECTOR3 cSzObjectContext::GetVec3Pos()
{
    D3DXVECTOR3 vec = { (float)objRect.left , (float)objRect.top , 0.0f };
    return vec;
}

cSzObjectContext* cSzObjectContext::CreateObjectNoSprite(std::vector<cSzObjectContext*>& pVec, int x, int y, int w, int h)
{
    objRect.left = x;
    objRect.top = y;
    objRect.right = w;
    objRect.bottom = h;

    sourceRect = { x,y,w,h };

    pVec.push_back(this);
    return this;
}

bool cSzObjectContext::IsInRect(int orgCursorX, int orgCursorY, int x, int y, int w, int h)
{
    bool result = false;
    if ((orgCursorX >= (x * GetScaleX()) &&
        orgCursorX <= ((x + w) * GetScaleX())) &&
        (orgCursorY >= (y * GetScaleY()) &&
            orgCursorY <= ((y + h) * GetScaleY())))
    {
        result = true;
    }
    return result;
}

bool cSzObjectContext::IsInRect(int orgCursorX, int orgCursorY)
{
    bool result = false;
    if ((orgCursorX >= (objRect.left * GetScaleX()) &&
        orgCursorX <= ((objRect.left + objRect.right) * GetScaleX())) &&
        (orgCursorY >= (objRect.top * GetScaleY()) &&
            orgCursorY <= ((objRect.top + objRect.bottom) * GetScaleY())))
    {
        result = true;
    }
    return result;
}

bool cSzObjectContext::IsInRect()
{
    auto point = cInputs->mousePos;
    return IsInRect(point.x, point.y);
}

bool cSzObjectContext::IsSelected()
{
    return isSelectedObject;
}

bool cSzObjectContext::IsVisible()
{
    return bIsVisible;
}

//METHODS
cSzObjectContext* cSzObjectContext::CreateTextureAndObject(LPDIRECT3DDEVICE9 pDevice, LPCSTR pSrcFile, std::vector<cSzObjectContext*>& pVec, int x, int y, int w, int h)
{
    if (SUCCEEDED(D3DXCreateTextureFromFileA(pDevice, pSrcFile, &pTexture)))
    {
        objRect.right = w;
        objRect.bottom = h;

        //calculo do objRect
        sourceRect = { x , y ,x + w , y + h };

        pVec.push_back(this);
    }
    return this;
}

cSzObjectContext* cSzObjectContext::CreateTexture(LPDIRECT3DDEVICE9 pDevice, LPCSTR pSrcFile)
{
    if (!SUCCEEDED(D3DXCreateTextureFromFileA(pDevice, pSrcFile, &pTexture)))
        printf("%s nao foi carregado\n", pSrcFile);

    return this;
}

void cSzObjectContext::DrawStdCabalTextBox(eCabal_WindowIdx mainWindow, int orientation, CABAL_STRING* pCabalString)
{
    //auto PrepareBox         = ((void(__cdecl*)(DWORD controler, RECT * rc, CABAL_STRING*, int, int orientation))0x00716258); //orientation 0 -> top / 1 -> right / 2 -> bottom
    //auto DrawCabalBox       = ((void(__thiscall*)(DWORD, CabalParentData*, RECT*))0x00758A30); //orientation 0 -> top / 1 -> right / 2 -> bottom
    //
    //RECT rc =
    //{
    //    static_cast<int>(this->objRect.left * GetScaleX()),
    //    static_cast<int>(objRect.top * GetScaleY()),
    //    static_cast<int>((objRect.left + objRect.right) * GetScaleX()),
    //    static_cast<int>((objRect.bottom + objRect.top) * GetScaleY())
    //};
    //
    //PrepareBox(DESC_CLEAR_SLOT, &rc, pCabalString, 0, orientation);
    //DrawCabalBox(0x010ACB80, g_CabalWindowData->GetWindowByID(mainWindow), &rc);
}

void cSzObjectContext::DrawCabalString()
{
    auto DrawCabalText2 = ((void(__thiscall*)(DWORD __thisPtr, const char* text, int maxCharsLimit, RECT * rc, DWORD textPosition, DWORD argbColor, int shadow, D3DXVECTOR3 * vec3, int unk2))0x00491504);

    if (!this->IsVisible())
        return;

    for (auto& texts : m_fontData)
    {
        if (!IsVisible())
            continue;

        auto sFontData = &texts.second;

        RECT fontRect =
        {
            static_cast<int>((objRect.left + sFontData->fontPosition.x) * ObjectScaleX),
            static_cast<int>((objRect.top + sFontData->fontPosition.y) * ObjectScaleY),
             sFontData->removeDelimitX ? MaxResolutionX : static_cast<int>((objRect.left + objRect.right + sFontData->fontPosition.x) * ObjectScaleX),
             sFontData->removeDelimitY ? MaxResolutionY : static_cast<int>((objRect.top + objRect.bottom + sFontData->fontPosition.y) * ObjectScaleY)
        };

        DrawCabalText2((*(DWORD**)0x00B8DAE4)[1537], sFontData->objText.c_str(), sFontData->fontDelimit, &fontRect, sFontData->fontPos, sFontData->colorRGBA, sFontData->useShadow, 0, 0);
    }
}



cSzObjectContext* cSzObjectContext::AddCallback(std::function<void(cSzObjectContext*, int, void*, WINPROC_CLICKIDX)> pCallback, int data1, void* data2, bool onRect)
{
    if (pCallback)
        vCallback.push_back({ std::bind(pCallback, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4) ,this, data1, data2, onRect });
    
    return this;
}

void cSzObjectContext::RunCallbacks(WINPROC_CLICKIDX clickType)
{
    for (auto& it2 : this->vCallback)
    {
        if (it2.onRect)
            continue;

        it2.callback(it2.parameter, it2.data1, it2.data2, clickType);
    }
}

void cSzObjectContext::RunCallbacksOnRect(bool isVisible)
{
    for (auto& it2 : this->vCallback)
    {
        if (!it2.onRect)
            continue;

        it2.callback(it2.parameter, isVisible, it2.data2, WINPROC_CLICKIDX::WP_UNUSED);
    }
}

cSzObjectContext* cSzObjectContext::RegisterObject(std::vector<cSzObjectContext*>& pVec)
{
    conditionType = eCondition::INGAME;

    pVec.push_back(this);
    return this;
}

cSzObjectContext* cSzObjectContext::RegisterObject(std::vector<cSzObjectContext*>& pVec, int windowId, int parentId)
{
    conditionType = eCondition::INGAME;
    windowIdx = windowId;
    parentIdx = parentId;
    pVec.push_back(this);
    return this;
}

cSzObjectContext* cSzObjectContext::AddComboBoxObj(std::shared_ptr<cSzObjectContext> pObjectCtx, int pos)
{
    m_ComboBoxObjects[pos] = pObjectCtx;

    return this;
}

cSzObjectContext* cSzObjectContext::RemComboBoxObj(int pos)
{
    if (m_ComboBoxObjects.contains(pos))
        m_ComboBoxObjects.erase(pos);

    return this;
}

cSzObjectContext* cSzObjectContext::SetComboVisibility(bool visibility)
{
    for (auto& it : m_ComboBoxObjects)
    {
        it.second->SetVisible(visibility);
    }
    return this;
}


cSzObjectContext* cSzObjectContext::CreateGenericButton(cSzObjectContext* spritePtr, std::vector<cSzObjectContext*>& pVec, eOBJTYPE objType, RECT normal, RECT hover, RECT clicked)
{
    ObjType = objType;
    pTexture = spritePtr->pTexture;

    objRect.right = normal.right;
    objRect.bottom = normal.bottom;

    buttonRect[0] = { normal.left,normal.top,normal.left + normal.right,normal.top + normal.bottom };
    buttonRect[1] = { hover.left,hover.top,hover.left + hover.right,hover.top + hover.bottom };
    buttonRect[2] = { clicked.left,clicked.top,clicked.left + clicked.right,clicked.top + clicked.bottom };

    pVec.push_back(this);
    return this;
}

cSzObjectContext* cSzObjectContext::Copy(cSzObjectContext* spritePtr, RECT* srcPos)
{
    pTexture = spritePtr->pTexture;
    objRect = spritePtr->objRect;
    conditionType = spritePtr->conditionType;

    //calculo do objRect
    sourceRect = { srcPos->left , srcPos->top ,srcPos->left + srcPos->right , srcPos->top + srcPos->bottom };
    return this;
}

cSzObjectContext* cSzObjectContext::RegisterExtraObj(cSzObjectContext* pExtraObj, bool pHidePreviousObject, bool hoverOnRect)
{
    extraObjPtr = pExtraObj;
    extraObjPtr->objRect = pExtraObj->objRect;
    extraObjPtr->isExtraObj = true;
    hidePreviousObject = pHidePreviousObject;
    this->hoverOnRect = hoverOnRect;
    return this;
}

cSzObjectContext* cSzObjectContext::SetFontSize(int fontSize)
{
    sFont.fontSize = fontSize;
    return this;
}

cSzObjectContext* cSzObjectContext::SetFormat(int format)
{
    sFont.iDT_Format = format;
    return this;
}

cSzObjectContext* cSzObjectContext::SetColor(D3DXCOLOR color)
{
    colorRGBA = color;
    return this;
}

cSzObjectContext* cSzObjectContext::EquipOnControl(WINPROC_CLICKIDX buttonType)
{
    eControlClicBtn = buttonType;
    return this;
}

void cSzObjectContext::ResizeWindow()
{
    //posição X do cursor + o tamanho do objeto menos o ponto em que foi clicado
    auto endPosX = (objRect.left + objRect.right);
    if (endPosX > (MaxResolutionX / GetScaleX()))
        objRect.left = (int)(MaxResolutionX / GetScaleX()) - objRect.right;

    //posição Y do cursor + o tamanho do objeto menos o ponto em que foi clicado
    auto endPosY = (objRect.top + objRect.bottom);
    if (endPosY > (MaxResolutionY / GetScaleY()))
        objRect.top = (int)(MaxResolutionY / GetScaleY()) - objRect.bottom;
}

void CustomDraw::SortIdxByUpperWindowOrParent(std::vector<cSzObjectContext*>& pVec)
{
    std::sort(pVec.begin(), pVec.end(),
        [](cSzObjectContext* pObj1, cSzObjectContext* pObj2)
        {
            //return pObj1->windowIdx < pObj2->windowIdx;
            if (pObj1->windowIdx == pObj2->windowIdx)
            {
                // Se parentIdx for o mesmo, então ordene por windowIdx
                return pObj1->parentIdx < pObj2->parentIdx;
            }
            else
            {
                // Caso contrário, ordene por parentIdx
                return pObj1->windowIdx < pObj2->windowIdx;
            }
        });
}

void CustomDraw::ScrollBarManager(int visibleRows, int visibleCols, int numObjects, cSzObjectContext* arrowUp, cSzObjectContext* arrowDown, cSzObjectContext* scroll, cSzObjectContext objects[], int basePosX, int basePosY, int leftSpace, int topSpace)
{
    if (!scroll->IsVisible())
        return;

    int& scrollPos = scroll->scrollpos;
    auto scrollSize = static_cast<float>(scroll->objRect.bottom);
    auto scrollworksize = static_cast<float>(arrowDown->objRect.top - (arrowUp->objRect.top + arrowUp->objRect.bottom) - scrollSize);

    if (!visibleCols)
        return;

    float rollMaxNum = static_cast<float>((numObjects / visibleCols) - visibleRows);
    scrollPos = min(scrollPos, rollMaxNum);
    scrollPos = max(scrollPos, 0);
    int resultPos = max(0, ((scrollworksize / rollMaxNum) * scrollPos));

    if (objects)
    {
        for (int i = 0; i < numObjects; i++)
        {
            int col = (i % visibleCols);
            int row = (i / visibleCols);

            if (row >= scrollPos && row < (scrollPos + visibleRows))
            {
                objects[i].SetVisible(true);
                objects[i].SetPositionL(basePosX + (col * (leftSpace + objects->objRect.right)), basePosY + (row - scrollPos) * (topSpace + objects->objRect.bottom));
            }
            else
            {
                objects[i].SetVisible(false);
            }
        }
    }

    scroll->SetPositionL(scroll->objRect.left, scroll->objRect.top + resultPos);

    return;
}

void CustomDraw::PushBackObjects(std::vector<cSzObjectContext*>& pVec, int windowIdx)
{
    auto iter = std::partition(pVec.begin(), pVec.end(),
        [&](const cSzObjectContext* data)
        {
            return data->windowIdx != windowIdx;
        });

    std::sort(iter, pVec.end(),
        [&](const cSzObjectContext* pObj1, const cSzObjectContext* pObj2)
        {
            // Ordena apenas se ambos tiverem o mesmo windowIdx
            if (pObj1->windowIdx == windowIdx && pObj2->windowIdx == windowIdx)
            {
                return pObj1->parentIdx < pObj2->parentIdx;
            }

            // Mantém a ordem original se os windowIdx forem diferentes
            return false;
        });
}

void CustomDraw::PushFrontObjects(std::vector<cSzObjectContext*>& pVec, int windowIdx)
{
    std::sort(pVec.begin(), pVec.end(),
        [&](const cSzObjectContext* pObj1, const cSzObjectContext* pObj2)
        {
            return pObj1->windowIdx < pObj1->windowIdx && pObj1->parentIdx < pObj2->parentIdx;
        });
}

void RegisterWindow(WINDOW_NAME_IDX windowIdx, bool useNewDraw, CustomDraw* pBaseClass, eVisibleType visibleType, eCondition condition, int drawPosWindowIdx)
{
    v_BaseObjects.push_back({ useNewDraw, pBaseClass, visibleType, condition, drawPosWindowIdx });
}

void DrawCustomWindowF(void* _this, BOOL isHide)
{
    for (auto& it : v_BaseObjects)
    {
        auto pWindow = &it;

        if (pWindow->baseClass->IsVisible() && !isHide && pWindow->condition == eCondition::INGAME && pWindow->visibleType == eVisibleType::SCREEN_FRONT)
            pWindow->baseClass->Draw();
    }
}

void DrawCustomWindowB(void* _this, BOOL isHide)
{
    for (auto& it : v_BaseObjects)
    {
        auto pWindow = &it;

        if (pWindow->baseClass->IsVisible() && !isHide && pWindow->condition == eCondition::INGAME && pWindow->visibleType == eVisibleType::SCREEN_BACK)
            pWindow->baseClass->Draw();
    }
}

void LoadCustomWindow()
{
    for (auto& it : textureIndex)
        mUI_Texture[it].CreateTexture(Direct3DDevice, std::format("{}\\Data\\ui\\theme1\\UI_Texture{}.dds", Basic::Game::GetGameDir(), it).c_str());

    for (auto& it : v_BaseObjects)
    {
        auto pWindow = &it;

        pWindow->baseClass->NotifyDevice();
        pWindow->baseClass->Present();
    }
}

void OnLostDeviceCustom()
{
    for (auto& it : cInputs->vArialFontSizes)
    {
        it.second->Release();						//chama o destrutor eliminando as fontes alocadas
        cInputs->vArialFontSizes.erase(it.first);	//limpa a class do unordered
    }
}

int MouseClickCustom(WPARAM wParam, LPARAM lParam)
{
    int result = true;

    cInputs->SetOrgCursorPos({ LOWORD(lParam), HIWORD(lParam) });


    for (auto& it : v_BaseObjects)
    {
        auto pWindow = &it;

        if (!pWindow->baseClass->IsVisible())
            continue;

        if (!pWindow->baseClass->MouseClick((WINPROC_CLICKIDX)wParam))
            result = FALSE;

        pWindow->baseClass->Present();
    }

    return result;
}

int MouseMoveCustom(WPARAM wParam, LPARAM lParam)
{
    int result = true;

    cInputs->SetOrgCursorPos({ LOWORD(lParam), HIWORD(lParam) });

    for (auto& it : v_BaseObjects)
    {
        auto pWindow = &it;

        if (!pWindow->baseClass->IsVisible())
            continue;

        if (!pWindow->baseClass->MouseMove(wParam))
        {
            result = FALSE;
        }

        pWindow->baseClass->Present();
    }

    return result;
}

void SaveOnCloseCustom()
{
    for (auto& it : v_BaseObjects)
    {
        auto pWindow = &it;
        pWindow->baseClass->SaveOnClose();
    }
}