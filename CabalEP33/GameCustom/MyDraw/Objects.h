#pragma once
#include <functional>
#include <algorithm>
#include "GameDef.h"
#include "../../DirectX/DirectX.h"
#include "DXColors.h"


const int textureIndex[] = { 0,1,4 };

enum WINDOW_NAME_IDX
{
    UI_BALDUSSYSTEM,
    UI_CASH_STORE,
    UI_COLLECTION,
    UI_CREATE_CHARACTER,
    UI_GLADIATOR_RAGE,
    UI_ITEM_SLOT,
    UI_BM2_MACRO,
    UI_BM3_MACRO,
    UI_REMOVE_BM_RIGHTCLICK,
    UI_MEDALOFHONOR,
    UI_SLOTREMOVER,
    UI_TGKILLSYSTEM,
    UI_TICKET,
    UI_UPGRADESYSTEM,
    UI_INFINITYBUFF
};

enum WINPROC_CLICKIDX
{
    WP_CLICKUP = 0,
    WP_LEFT_CLICKDOWN = 1,
    WP_RIGHT_CLICKDOWN = 2,

    WP_SHIFT_CLICKUP = 4,
    WP_SHIFT_LEFT_CLICKDOWN = 5,
    WP_SHIFT_RIGHT_CLICKDOWN = 6,

    WP_CONTROL_CLICKUP = 8,
    WP_CONTROL_LEFT_CLICKDOWN = 9,
    WP_CONTROL_RIGHT_CLICKDOWN = 10,
    WP_MOUSEWHEELUP = 0xFE,
    WP_MOUSEWHEELDOWN = 0xFF,
    WP_UNUSED = 1000
};

enum eVisibleType
{
    SCREEN_BACK,
    SCREEN_FRONT
};

enum eCondition
{
    INGAME,
    SELECTION_SCREEN,
    OPEN_INVENTORY
};

enum eOBJTYPE
{
    NONE,
    GENERIC_BUTTON_SAVESTATE,
    GENERIC_BUTTON_NOSAVE,
    NORMAL_HINT,
    SIMPLE_HINT,
    GROUP_OBJECTS,
    SCROLABLE,
};

enum eBTN_TYPES
{
    E_BTN_NORMAL,
    E_BTN_CLICKED,
    E_BTN_ONRECT,
};

enum CabalTextAlignment
{
    LEFT = 0,
    CENTER_LEFT = 1,
    RIGHT = 2,
    CENTER_LEFT2 = 3,
    CENTER_TOP = 4,
    FULLCENTER = 5,
    CENTER_TOP_RIGHT = 6,
    CENTER3 = 7,
    QUARTER_TOP_LEFT = 8,
    QUARTER_CENTER_AGAIN = 9,
    QUARTER_RIGHT_BOTTOM = 10,
    QUARTER_CENTER_BOTTOM = 11,

    LEFT_BOTTOM = 12,
    CENTER_BOTTOM_AGAIN = 13,
    RIGHT_BOTTOM = 14,
    CENTER_BOTTOM = 15
};

enum eCabal_WindowIdx
{
	NULO								= 0,
	UNKNOWN_LASTUI						= 0,			//
	SELECTION_CHANNEL					= 3300000,
	INVENTORY							= 10400000,
	BTN_OPEN_INVENTORYCASH				= 11400011,
	EXTRACTITEM							= 10400013,			//botão de extração de itens
	LOJA_N								= 13000000,
	OPEN_CASH_INVENTORY_NOTIFICATION	= 15100000,
	INVENTORY_CASH						= 22200000,
	OPENUPGRADE							= 15400000,
	SUBPASSWORD							= 16200000,
	BOXMENU_SELECTION					= 34100000,
	ITEM_UNIT							= 40000000,
	DUNGEONEND_DIALOG					= 31900000,
	SKILL_BOXINFO						= 40600000,
	DESC_CLEAR_SLOT						= 49000000,			//box mostrada quando o slot está vazio
	LOJA_PESSOAL						= 24400000,			//slotar no NPC
	ENCHANT_ITEM_SLOT					= 24400000,			//slotar no NPC
	ENCHANT_PET							= 26800000,			//slotar pet
	CHANGE_SHAPE						= 27300000,			//Mudar aparencia
	TRADE_ITEM							= 14300000,			//ao fazer trocas
	SEND_EMAIL							= 36800000,			//ao enviar emails
	LEILAO								= 17600000,			//ao enviar emails
	GUILD_BAU							= 16600000,			//ao enviar emails
	MAP_UI								= 25700000,			//Mapa atalho M
	HP_BAR								= 500000,			//
	SERVICE_PREMIUM_ICONS				= 900000,			//
	HP_MP_SPBAR							= 2000000,			//
	MINIMAP								= 2200000,			//
	SKILLBAR							= 3200000,			//
	CHAT_AND_SYSTEM						= 4400000,			//
	JUNKEBOX							= 10600000,			//
	SPECIALITEMNOTIFY					= 15900000,			//
	EMAILNOTIFY							= 26600000,			//
};


#pragma pack(push,1)
struct CABAL_STRING
{
    int align;
    char text[16];
    int textSize;
    int textMaxSize;

    const char* GetText()
    {
        std::string tmpText;
        if (textMaxSize < 16)
        {
            tmpText.append(text, textSize);
            return tmpText.c_str();
        }
        return *(const char**)text;
    }

    void CabalLoadString(const char* text)
    {
        //((void(__thiscall*)(CABAL_STRING*, const char*))0x0044DFB6)(this, text);
    }

};
#pragma pack(pop)

//definição de struct
struct S_Callback;

class cSzFont
{
public:
    LPD3DXFONT myFont = 0;
    int iSize = 0;

    cSzFont() : myFont(0), iSize(0) {}

    void CreateMyFont(int size)
    {
        iSize = size;
        D3DXCreateFont(*(LPDIRECT3DDEVICE9*)0x00CEB7F4, size, 0, FW_EXTRALIGHT, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLEARTYPE_NATURAL_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial", &myFont);
    }

    void Release()
    {
        if (myFont)
        {
            myFont->OnLostDevice(); //libera os objetos
            myFont->OnResetDevice(); //libera os objetos
            myFont->Release(); //limpa a memoria
        }
    }
};

class cSzInputData
{
public:
    bool bKeyBoardStatus[256] = { 0 };
    bool bMouseStatus[15] = { 0 };

    POINT mousePos;
    POINTS wOrgMousePosition;
    std::unordered_map<int, std::unique_ptr<cSzFont>> vArialFontSizes;

    POINT getOrgCursorPos() { return mousePos; }
    POINTS wGetOrgCursorPos() { return wOrgMousePosition; }

    bool IsLettersAndNumbers(WPARAM wParam)
    {
        if ((wParam >= 48 && wParam <= 57) || (wParam >= 65 && wParam <= 90) || (wParam >= 97 && wParam <= 122))
            return true;
        else
            return false;
    }

    void SetOrgCursorPos(POINT point)
    {
        mousePos = point;
        wOrgMousePosition = { (SHORT)point.x,(SHORT)point.y };
    }

    bool IsPressedKeyMouse(WINPROC_CLICKIDX button)
    {
        return bMouseStatus[button];
    }

    bool IsPressedKeyBoard(int position)
    {
        if (position < 0 || position > 256)
            return false;

        return bKeyBoardStatus[position];
    }

    auto GetFontBySize(int fontSize)
    {
        if (!vArialFontSizes.contains(fontSize))
        {
            //printf("total de fontes no mapa %d\n", vArialFontSizes.size());
            vArialFontSizes[fontSize].reset(new cSzFont);
            vArialFontSizes[fontSize].get()->CreateMyFont(fontSize);
        }
        return vArialFontSizes[fontSize].get()->myFont;
    }
};

struct TextureList
{
    LPDIRECT3DTEXTURE9 pTexture = 0;
    RECT sourceRect;    //objRect com recorte do arquivo carregado
    int addXPos;
    int addYPos;
    float scale = 0.0f;
    bool isHover = false;
    bool viewOnClick = false;
    bool useEventMouse = false;
    bool visible = true;

    void SetW(int width)
    {
        sourceRect.right = sourceRect.left + width;
    }

    void SetH(int height)
    {
        sourceRect.bottom = sourceRect.right + height;
    }

    float GetScale()
    {
        return ObjectScaleX + ((ObjectScaleX / 100.0f) * scale);
    }
};

struct FontData
{
    bool calculated = false;

    bool removeDelimitX = false; //deixa a delimitação de texto como maxresolution
    bool removeDelimitY = false; //deixa a delimitação de texto como maxresolution

    RECT fontRectangle = { 0,0,0,0 };

    //texto estático armazenado pelo objeto
    std::string objText;

    //texto do input
    std::string inputText;

    //tipo de format que é usado na drawtext
    int iDT_Format = 0;

    //tamanho da fonte
    int fontSize = 0;

    bool autoScale = false;

    POINT fontPosition = { 0,0 };

    bool isOnlyFont = false;

    //usada na nova draw de texto
    int fontPos = 0;

    BOOL useShadow;

    DWORD colorRGBA;

    int fontDelimit = -1;

};

class cSzObjectContext : public cSzInputData
{
public:
    //identificadores para grupos
    int groupIdx = -1;
    int rowIdx = -1;
    int colIdx = -1;
    int scrollpos = 0;
    int scrollMax = 0;

    //caso seja slots
    int slotIdx = -1;

    void* p_AddtionalData = 0;
    void* p_AddtionalData2 = 0;

    eCondition conditionType;
    eOBJTYPE ObjType        = NONE;
    eOBJTYPE ObjSubType     = NONE;

    int windowIdx           = 0;
    int parentIdx           = 0;

    //cor do objeto
    D3DXCOLOR colorRGBA = D3DCOLOR_RGBA(255, 255, 255, 255);

    //ponteiro da textura carregada
    LPDIRECT3DTEXTURE9 pTexture = 0;

    POINT pointClick;   //ponto onde foi feito o click
    RECT buttonRect[3]; //objRect com recorte do arquivo carregado -> 0 = normal, 1 = hover, 2 = selected
    RECT sourceRect;    //objRect com recorte do arquivo carregado
    RECT objRect;       //coordenadas, tamanho e posição do clique no objeto

    std::map<int,TextureList> m_textureData;
    std::map<int, FontData> m_fontData;
    FontData sFont;

    std::map<int, std::shared_ptr<cSzObjectContext>> m_ComboBoxObjects;

    //obtém o dxvecto3 para a draw
    D3DXVECTOR3 GetVec3Pos();

    //lista de callbacks do objeto
    std::vector<S_Callback> vCallback;
    std::function<void(cSzObjectContext*, WINPROC_CLICKIDX)> itemCallback;

    //registra um objeto adicional usado para botões ao passar o mouse por cima
    cSzObjectContext* extraObjPtr = 0;


    bool bIsHint = false;
    bool IsHint() { return bIsHint; }

    CABAL_STRING hintText;

    //dados do text box
    struct S_TEXTBOX
    {
        cSzObjectContext* img = 0;
        std::string title;
        std::string text;
    }sTextBox;

    bool hidePreviousObject = false;

    //true - quando 
    bool bIsVisible = false;

    //true - para quando o sprite está sendo movido
    bool moveOnClick        = false;
    bool isSelectedObject   = false;
    bool clicked            = false;
    bool hoverOnRect        = true; //faz o evento se sobreposição quando o mouse está em cima


    bool inRect = false;

    //scaling da imagem
    float fProportionPercent = 0.0f;
    D3DXMATRIX objMatrix;

    //flag usada para ignorar os objetos extra pois os clicks são colocados no objeto pai
    bool isExtraObj = false;

    //salva o estado de click ao pressionar control
    WINPROC_CLICKIDX eControlClicBtn = WINPROC_CLICKIDX::WP_UNUSED;

    //recalcula e obtem o tamanho do retangulo do texto
    cSzObjectContext* DrawString();

    //seta se o botão está clicado ou não
    void SetSelected(bool status);

    //obtem o estado do click no botão
    bool IsSelected();

    //retorna verdadeiro caso o cursor esteja em cima do retangulo
    bool IsInRect();
    bool IsInRect(int orgCursorX, int orgCursorY);
    bool IsInRect(int orgCursorX, int orgCursorY, int x, int y, int w, int h);

    //ajusta a UI de acordo com a resolução para evitar que os objetos sumam no caso de uma mudança de resolução
    void ResizeWindow();

    //checa se o objeto está visivel
    bool IsVisible();

    //Draw Cabal Text
    void DrawStdCabalTextBox(eCabal_WindowIdx mainWindow, int orientation, CABAL_STRING* pCabalString);

    //desenha o texto padrão do cabal
    void DrawCabalString();

    cSzObjectContext* AddCallback(std::function<void(cSzObjectContext*, int, void*, WINPROC_CLICKIDX)> pCallback, int data1, void* data2 = nullptr, bool onRect = false);

    void RunCallbacks(WINPROC_CLICKIDX clickType);
    void RunCallbacksOnRect(bool isVisible);

    //registra um objeto adicional usado para botões ao passar o mouse por cima / "pHidePreviousObject = true"  diz se o objeto anterior não será apresentado 
    cSzObjectContext* RegisterExtraObj(cSzObjectContext* pExtraObj, bool pHidePreviousObject, bool hoverOnRect = true);

    //carrega um sprite e pode compartilhar com outros sprites através de sobrecarga
    cSzObjectContext* CreateTexture(LPDIRECT3DDEVICE9 pDevice, LPCSTR pSrcFile);

    //carrega a textura configura as informações além de insertar os objetos na lista
    cSzObjectContext* CreateTextureAndObject(LPDIRECT3DDEVICE9 pDevice, LPCSTR pSrcFile, std::vector<cSzObjectContext*>& pVec, int x, int y, int w, int h);

    //apenas copia a posição de as propriedades da base, não aceita interações como objetos
    cSzObjectContext* Copy(cSzObjectContext* spritePtr, RECT* srcPos);


    //Inicializa meu proprio objeto
    cSzObjectContext* RegisterObject(std::vector<cSzObjectContext*>& pVec);
    cSzObjectContext* RegisterObject(std::vector<cSzObjectContext*>& pVec, int windowId, int parentId = 0);

    //combo box
    cSzObjectContext* AddComboBoxObj(std::shared_ptr<cSzObjectContext> pVec, int pos);

    //apaga o objeto no mapa
    cSzObjectContext* RemComboBoxObj(int pos);

    //seta a visibilidade
    cSzObjectContext* SetComboVisibility(bool visibility);

    //cria um botão generico com 3 tipos
    cSzObjectContext* CreateGenericButton(cSzObjectContext* spritePtr, std::vector<cSzObjectContext*>& pVec, eOBJTYPE objType, RECT normal, RECT hover, RECT clicked);

    cSzObjectContext* SetW(int width);
    cSzObjectContext* SetH(int height);
    cSzObjectContext* SetSourceW(int width);
    cSzObjectContext* SetSourceH(int height);

    //ajusta a posição na tela do lado esquerdo pro direito
    cSzObjectContext* SetPositionL(int x, int y);

    //centraliza em w/h o objeto o mantando totalmente no centro da janela
    cSzObjectContext* SetWindowCenter(int x, int y);

    cSzObjectContext* SetPositionExtraObj_L(int x, int y);

    //cria botões gerenciáveis que salvam o estado do click e alternam
    cSzObjectContext* OnSwitchClickInObject(void* referenceOfVarInClass);

    //ajusta a posição na tela do lado direito pro esquerdo
    cSzObjectContext* SetPositionR(int x, int y);

    //Ajusta a UI com a partir do right/bottom
    cSzObjectContext* SetPositionRT(int x, int y);

    //ajusta a posição do objeto extra
    cSzObjectContext* SetPositionExtraObj_R(int x, int y);

    //ajusta a posição no centro da tela W / H 
    cSzObjectContext* SetPositionCenterW(int x, int y);
    cSzObjectContext* SetPositionCenterH(int x, int y);

    //alterna a visibilidade do objeto
    cSzObjectContext* SetVisible(bool status);

    //atualiza o tamanho do objeto
    cSzObjectContext* SetSize(int w, int h);

    //"alpha" -> ajusta a transparência do objeto // "alphaModifier" -> tipo de modificador de alpha -> 0 não usa modificadores - 4 remove a transparencia
    cSzObjectContext* SetAlpha(unsigned char alpha);

    //o objeto pode mover com o cursor?
    cSzObjectContext* SetMove();

    //cria um texto a parte separado de sprite
    cSzObjectContext* CreateText(std::vector<cSzObjectContext*>& pVec, std::string text, int pDT_Format, int fontSize, D3DXCOLOR color, int pAddPosX = 0, int pAddPosY = 0, bool useDtCalcRect = false, int x = 0, int y = 0);
    cSzObjectContext* CreateText(std::string text, int pDT_Format, int fontSize, D3DXCOLOR color, RECT textRect = {0,0,0,0}, int pAddPosX = 0, int pAddPosY = 0, bool useDtCalcRect = false);

    //adiciona texto a um objeto que pode conter sprite
    cSzObjectContext* AddText(std::string text, int pDT_Format, int fontSize, D3DXCOLOR color, int pAddPosX = 0, int pAddPosY = 0);
    cSzObjectContext* AddText(std::string text, RECT textRect, int pDT_Format, int fontSize, D3DXCOLOR color, int pAddPosX = 0, int pAddPosY = 0);
    cSzObjectContext* AddText(std::string text, RECT textRect, bool removeDelimitX, bool removeDelimitY, int pDT_Format, int fontSize, D3DXCOLOR color, int pAddPosX = 0, int pAddPosY = 0);
   

    cSzObjectContext* AddText(int order, std::string text, D3DXCOLOR color, int pAddPosX, int pAddPosY, CabalTextAlignment fontPos, bool useShadow);
    cSzObjectContext* AddText(int order, std::string text, RECT textRect, D3DXCOLOR color, int pAddPosX, int pAddPosY, CabalTextAlignment fontPos, bool useShadow);

    
    //muda o texto interno do objeto
    cSzObjectContext* SetText(std::string text);

    //muda o texto interno do objeto de input e recalcula o tamanho da objRect da fonte
    cSzObjectContext* SetInputText(std::string text);

    //define o tamanho do retangulo
    cSzObjectContext* CreateObjectNoSprite(std::vector<cSzObjectContext*>& pVec, int x, int y, int w, int h);

    /****************************************** FONTES ********************************************/
    //muda o tamanho da fonte
    cSzObjectContext* SetFontSize(int fontSize);

    //define o tipo de formatação de texto
    cSzObjectContext* SetFormat(int format);

    //define a cor do texto
    cSzObjectContext* SetColor(D3DXCOLOR color);

    //se definido ao usar control + tecla e a UI estiver selecionada, move o item para o slot
    cSzObjectContext* EquipOnControl(WINPROC_CLICKIDX buttonType);

    void DrawItemDescription();

    //desenha uma box
    void DrawBox(float x, float y, float w, float h, D3DCOLOR Color);
    void DrawBox(RECT* rc, D3DCOLOR Color);
    cSzObjectContext* AddTexture(int index, cSzObjectContext& pObjectRef, RECT sourceRect, int addXPos = 0, int addYPos = 0, float scale = 0.0f, bool isHover = false, bool viewOnClick = false, bool useMouseEvents = false, bool visible = true);
    cSzObjectContext* AddTexture(int index, LPDIRECT3DTEXTURE9 pTexture, RECT sourceRect, int addXPos = 0, int addYPos = 0, float scale = 0.0f, bool isHover = false, bool viewOnClick = false, bool useMouseEvents = false, bool visible = true);
    cSzObjectContext* AddTexture(int index, const char* pTextureFolder, RECT sourceRect, int addXPos = 0, int addYPos = 0, float scale = 0.0f, bool isHover = false, bool viewOnClick = false, bool useMouseEvents = false, bool visible = true);

    //apaga um objeto
    cSzObjectContext* EraseTexture(int index);

    void DrawBox(D3DCOLOR Color);

    //desenha uma box com borda
    void DrawBorderBox(int x, int y, int w, int h, D3DCOLOR lineColor, D3DCOLOR boxColor, int lineWeight);

    //desenha os tipos de botões definidos no ENUM
    void DrawButton();

    //desenha uma box apenas com as linhas
    void DrawBoxLine(int x, int y, int w, int h, int lineWeight, D3DCOLOR Color);

    //desenha uma textBox semelhante ao do cabal
    void SetTitleCabalHint(std::string title);

    void SetTextCabalHint(std::string text);

    //desenha a hint
    void CabalHint();

    void CreateCabalHint(std::vector<cSzObjectContext*>& pVec, eOBJTYPE objType, int x, int y, int w, int h, std::string title, std::string text, cSzObjectContext* image, int fontSize);
    void CreateCabalHint(eOBJTYPE objType, int x, int y, int w, int h, std::string title, std::string text, cSzObjectContext* image, int fontSize);

    cSzObjectContext* SetObjectType(eOBJTYPE objType)
    {
        ObjType = objType;
        return this;
    }

    //obtem a escala aplicando o calculo de proporção
    float GetScaleX();
    float GetScaleY();
    cSzObjectContext* SetProportion(float percent);

    /***********************************************************************************************/
};

struct S_Callback
{
    std::function<void(cSzObjectContext*, int, void*, WINPROC_CLICKIDX)> callback;
    cSzObjectContext* parameter = 0;
    int data1 = 0;
    void* data2 = 0;
    bool onRect = false;
};

class CustomDraw
{
public:
    virtual void NotifyDevice() = 0;
    virtual void Present() = 0;
    virtual void Draw() = 0;
    virtual void Release() = 0;
    virtual LRESULT CALLBACK OnKeyBoardEvent(WPARAM wParam, WPARAM wParamTranslated) = 0;
    virtual LRESULT CALLBACK MouseClick(WINPROC_CLICKIDX state) = 0;
    virtual LRESULT CALLBACK MouseMove(WPARAM wParam) = 0;
    virtual bool IsVisible() = 0;
    virtual void SaveOnClose() = 0;
    virtual void ResizeWindow() = 0;
    //virtual void CabalParentEvent(CabalParentData* parentPtr, BOOL& isVisible) = 0;
    virtual std::vector<cSzObjectContext*> GetVectorObjects() = 0;


    void PushBackObjects(std::vector<cSzObjectContext*>& pVec, int windowIdx);
    void PushFrontObjects(std::vector<cSzObjectContext*>& pVec, int windowIdx);
    void SortIdxByUpperWindowOrParent(std::vector<cSzObjectContext*>& pVec);

    void ScrollBarManager(int visibleRows, int visibleCols, int numObjects, cSzObjectContext* arrowUp, cSzObjectContext* arrowDown, cSzObjectContext* scroll, cSzObjectContext objects[], int basePosX, int basePosY, int leftSpace, int topSpace);

};

struct S_DrawBase
{
    bool useNewDraw             = false;
    CustomDraw*  baseClass      = 0;
    eVisibleType visibleType;
    eCondition condition;
    int drawPosWindowIdx        = 0;
};

void RegisterWindow(WINDOW_NAME_IDX windowIdx, bool useNewDraw, CustomDraw* pBaseClass, eVisibleType visibleType, eCondition condition, int drawPosWindowIdx = 0);

void DrawCustomWindowF(void* _this, BOOL isHide);

void DrawCustomWindowB(void* _this, BOOL isHide);

void LoadCustomWindow();

void OnLostDeviceCustom();

int MouseClickCustom(WPARAM wParam, LPARAM lParam);

int MouseMoveCustom(WPARAM wParam, LPARAM lParam);

void SaveOnCloseCustom();

//para inputs
extern cSzInputData* cInputs;
//para todas bases de objetos
extern std::vector<S_DrawBase> v_BaseObjects;
//para as UI da pasta theme1
extern std::unordered_map<int, cSzObjectContext> mUI_Texture;