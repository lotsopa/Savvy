#include "SavvyAppFrame.h"

// Register all events for this frame class
wxBEGIN_EVENT_TABLE(SavvyEditor::AppFrame, wxFrame)
EVT_MENU(wxID_EXIT, SavvyEditor::AppFrame::OnExit)
EVT_MENU(wxID_ABOUT, SavvyEditor::AppFrame::OnAbout)
EVT_MENU(wxID_OPEN, SavvyEditor::AppFrame::OnFileOpen)
EVT_MENU(wxID_NEW, SavvyEditor::AppFrame::OnFileNew)
EVT_MENU(wxID_SAVE, SavvyEditor::AppFrame::OnFileSave)
EVT_MENU(wxID_SAVEAS, SavvyEditor::AppFrame::OnFileSaveAs)
EVT_MENU(wxID_CLOSE, SavvyEditor::AppFrame::OnFileClose)
EVT_MENU(wxID_UNDO, SavvyEditor::AppFrame::OnUndo)
EVT_MENU(wxID_REDO, SavvyEditor::AppFrame::OnRedo)
EVT_MENU(wxID_CUT, SavvyEditor::AppFrame::OnCut)
EVT_MENU(wxID_COPY, SavvyEditor::AppFrame::OnCopy)
EVT_MENU(wxID_PASTE, SavvyEditor::AppFrame::OnPaste)
EVT_MENU(wxID_DELETE, SavvyEditor::AppFrame::OnDelete)
EVT_MENU(wxID_SELECTALL, SavvyEditor::AppFrame::OnSelectAll)
EVT_MENU(ID_Convert, SavvyEditor::AppFrame::OnConvert)
EVT_MENU(ID_GLSL, SavvyEditor::AppFrame::OnLangSelectGLSL)
EVT_MENU(ID_HLSL, SavvyEditor::AppFrame::OnLangSelectHLSL)
EVT_MENU(ID_None, SavvyEditor::AppFrame::OnLangSelectNone)
EVT_SIZE(SavvyEditor::AppFrame::OnResize)
EVT_TEXT(ID_TextAreaUser, SavvyEditor::AppFrame::OnTextChanged)
EVT_MENU_OPEN(SavvyEditor::AppFrame::OnMenuOpen)
EVT_MENU(ID_FindDialog, SavvyEditor::AppFrame::OnShowFindDialog)
EVT_MENU(ID_ReplaceDialog, SavvyEditor::AppFrame::OnShowReplaceDialog)
EVT_FIND(wxID_ANY, SavvyEditor::AppFrame::OnFindDialog)
EVT_FIND_NEXT(wxID_ANY, SavvyEditor::AppFrame::OnFindDialog)
EVT_FIND_REPLACE(wxID_ANY, SavvyEditor::AppFrame::OnFindDialog)
EVT_FIND_REPLACE_ALL(wxID_ANY, SavvyEditor::AppFrame::OnFindDialog)
EVT_FIND_CLOSE(wxID_ANY, SavvyEditor::AppFrame::OnFindDialog)
wxEND_EVENT_TABLE()

SavvyEditor::AppFrame::AppFrame(const wxString& a_Title, const wxPoint& a_Pos, const wxSize& a_Size)
: wxFrame(NULL, wxID_ANY, a_Title, a_Pos, a_Size), m_TextAreaUser(NULL), m_ReplaceDialog(NULL), m_FindDialog(NULL), m_CurrFindPos(0)
{
	// Center on screen
	Centre();

	// Set an Icon
	SetIcon(wxIcon(wxT("res/Savvy_Logo.xpm"), wxBITMAP_TYPE_XPM));

	// Reset the current File being edited
	m_CurrDocPath = DEFAULT_DOC_PATH;

	// Construct the File menu options
	m_FileMenu = new wxMenu();
	m_FileMenu->Append(wxID_NEW, "&New\tCtrl+N", "Create a new file");
	m_FileMenu->Append(wxID_OPEN, "&Open\tCtrl+O", "Open a file");
	m_FileMenu->Append(wxID_SAVE, "&Save\tCtrl+S", "Save current file to disk...");
	m_FileMenu->Append(wxID_SAVEAS, "Save &As\tCtrl+Alt+S", "Save current file to disk as...");
	m_FileMenu->Append(wxID_CLOSE, "&Close\tCtrl+W", "Close the current file");
	m_FileMenu->AppendSeparator();
	m_FileMenu->Append(wxID_EXIT, "&Quit\tAlt+F4", "Quit Savvy Editor");

	// Edit Menu
	m_EditMenu = new wxMenu();
	m_EditMenu->Append(wxID_UNDO);
	m_EditMenu->Append(wxID_REDO);
	m_EditMenu->AppendSeparator();
	m_EditMenu->Append(wxID_CUT);
	m_EditMenu->Append(wxID_COPY);
	m_EditMenu->Append(wxID_PASTE);
	m_EditMenu->Append(wxID_DELETE, "&Delete\tDEL");
	m_EditMenu->Append(wxID_SELECTALL, "Select &All\tCtrl+A", "Select all the text in the document");

	// Search menu
	m_SearchMenu = new wxMenu();
	m_SearchMenu->AppendCheckItem(ID_FindDialog, wxT("&Find\tCtrl-F"));
	m_SearchMenu->AppendCheckItem(ID_ReplaceDialog, wxT("Find and &Replace\tCtrl-Shift-F"));

	// Language Menu
	m_LanguageMenu = new wxMenu();
	m_LanguageMenu->Append(ID_GLSL, "&GLSL", "Enable GLSL syntax highlighting");
	m_LanguageMenu->Append(ID_HLSL, "&HLSL", "Enable HLSL syntax highlighting");
	m_LanguageMenu->Append(ID_None, "&None", "Disable syntax highlighting");

	// Help Menu
	m_HelpMenu = new wxMenu();
	m_HelpMenu->Append(wxID_ABOUT);

	// Convert Menu
	m_ConvertMenu = new wxMenu();
	m_ConvertMenu->Append(ID_Convert, "&Convert\tF1", "Brings up the conversion options for the file");

	// Create the toolbar that's going to contain the menus
	m_MenuBar = new wxMenuBar();
	m_MenuBar->Append(m_FileMenu, "&File");
	m_MenuBar->Append(m_EditMenu, "&Edit");
	m_MenuBar->Append(m_SearchMenu, "&Search");
	m_MenuBar->Append(m_LanguageMenu, "&Language");
	m_MenuBar->Append(m_ConvertMenu, "&Conversion");
	m_MenuBar->Append(m_HelpMenu, "&Help");

	SetMenuBar(m_MenuBar);

	// GLSL Language Keywords
	m_GLSLKeyWords.Append("return for while break continue if do true false const int uint float void char double bool vec2 vec3 vec4 ivec2 ivec3 ivec4 uvec2 uvec3 uvec4 bvec2 bvec3 bvec4 struct ");
	m_GLSLKeyWords.Append("mat2 mat3 mat4 mat2x2 mat2x3 mat2x4 mat3x2 mat3x3 mat3x4 mat4x2 mat4x3 mat4x4 uniform varying in out inout layout binding location std430 std140 packed shared ");
	m_GLSLKeyWords.Append("sampler1D sampler2D sampler3D samplerCube sampler2DRect sampler1DArray sampler2DArray samplerCubeArray samplerBuffer sampler2DMS sampler2DMSArray ");
	m_GLSLKeyWords.Append("usampler1D usampler2D usampler3D usamplerCube usampler2DRect usampler1DArray usampler2DArray usamplerCubeArray usamplerBuffer usampler2DMS usampler2DMSArray ");
	m_GLSLKeyWords.Append("isampler1D isampler2D isampler3D isamplerCube isampler2DRect isampler1DArray isampler2DArray isamplerCubeArray isamplerBuffer isampler2DMS isampler2DMSArray ");
	m_GLSLKeyWords.Append("sampler1DShadow sampler2DShadow samplerCubeShadow sampler2DRectShadow sampler1DArrayShadow sampler2DArrayShadow samplerCubeArrayShadow buffer flat noperspective smooth centroid sample");

	//  GLSL Language Functions
	m_GLSLFuncs.Append("abs acos acosh all any asin asinh atan atanh atomicAdd atomicAnd atomicCompSwap atomicCounter atomicCounterDecrement atomicCounterIncrement atomicExchange atomicMax atomicMin atomicOr atomicXor ");
	m_GLSLFuncs.Append("barrier bitCount bitfieldExtract bitfieldInsert bitfieldReverse ");
	m_GLSLFuncs.Append("ceil clamp cos cosh cross degrees determinant dFdx dFdxCoarse dFdxFine dFdy dFdyCoarse dFdyFine distance dot ");
	m_GLSLFuncs.Append("EmitStreamVertex EmitVertex EndPrimitive EndStreamPrimitive equal exp exp2 faceforward findLSB findMSB floatBitsToInt floatBitsToUint floor fma fract frexp fwidth fwidthCoarse fwidthFine ");
	m_GLSLFuncs.Append("gl_ClipDistance gl_CullDistance gl_FragCoord gl_FragDepth gl_FrontFacing gl_GlobalInvocationID gl_HelperInvocation gl_InstanceID gl_InvocationID gl_Layer gl_LocalInvocationID gl_LocalInvocationIndex "
		"gl_NumSamples gl_NumWorkGroups gl_PatchVerticesIn gl_PointCoord gl_PointSize gl_Position gl_PrimitiveID gl_PrimitiveIDIn gl_SampleID gl_SampleMask gl_SampleMaskIn gl_SamplePosition gl_TessCoord gl_TessLevelInner "
		"gl_TessLevelOuter gl_VertexID gl_ViewportIndex gl_WorkGroupID gl_WorkGroupSize greaterThan greaterThanEqual groupMemoryBarrier ");
	m_GLSLFuncs.Append("imageAtomicAdd "
		"imageAtomicAnd "
		"imageAtomicCompSwap "
		"imageAtomicExchange "
		"imageAtomicMax "
		"imageAtomicMin	"
		"imageAtomicOr "
		"imageAtomicXor "
		"imageLoad "
		"imageSamples "
		"imageSize "
		"imageStore "
		"imulExtended "
		"intBitsToFloat "
		"interpolateAtCentroid "
		"interpolateAtOffset "
		"interpolateAtSample "
		"inverse "
		"inversesqrt "
		"isinf "
		"isnan ");
	m_GLSLFuncs.Append("ldexp length lessThan lessThanEqual log log2 ");
	m_GLSLFuncs.Append("matrixCompMult max memoryBarrier memoryBarrierAtomicCounter memoryBarrierBuffer memoryBarrierImage memoryBarrierShared min mix mod modf ");
	m_GLSLFuncs.Append("noise noise1 noise2 noise3 noise4 normalize not notEqual outerProduct packDouble2x32 packHalf2x16 packSnorm2x16 packSnorm4x8 packUnorm packUnorm2x16 packUnorm4x8 pow ");
	m_GLSLFuncs.Append("radians reflect refract round roundEven sign sin sinh smoothstep sqrt step ");
	m_GLSLFuncs.Append("tan "
		"tanh "
		"texelFetch "
		"texelFetchOffset "
		"texture "
		"textureGather "
		"textureGatherOffset "
		"textureGatherOffsets "
		"textureGrad "
		"textureGradOffset "
		"textureLod "
		"textureLodOffset "
		"textureOffset "
		"textureProj "
		"textureProjGrad "
		"textureProjGradOffset "
		"textureProjLod "
		"textureProjLodOffset "
		"textureProjOffset "
		"textureQueryLevels "
		"textureQueryLod "
		"textureSamples "
		"textureSize "
		"transpose "
		"trunc ");
	m_GLSLFuncs.Append("uaddCarry uintBitsToFloat umulExtended unpackDouble2x32 unpackHalf2x16 unpackSnorm2x16 unpackSnorm4x8 unpackUnorm unpackUnorm2x16 unpackUnorm4x8 usubBorrow");
	
	// HLSL Key Words
	m_HLSLKeyWords.Append("return for while break continue if do true false const int uint float void char double bool Buffer StructuredBuffer dword half snorm unorm struct ");
	m_HLSLKeyWords.Append("min16float min10float min16int min12int min16uint string vector matrix bool1 bool2 bool3 bool4 int1 int2 int3 int4 uint1 uint2 uint3 uint4 ");
	m_HLSLKeyWords.Append("float1 float2 float3 float4 double1 double2 double3 double4 int1x1 int1x2 int1x3 int1x4 int2x1 int2x2 int2x3 int2x4 int3x1 int3x2 int3x3 int3x4 int4x1 int4x2 int4x3 int4x4 ");
	m_HLSLKeyWords.Append("uint1x1 uint1x2 uint1x3 uint1x4 uint2x1 uint2x2 uint2x3 uint2x4 uint3x1 uint3x2 uint3x3 uint3x4 uint4x1 uint4x2 uint4x3 uint4x4 ");
	m_HLSLKeyWords.Append("float1x1 float1x2 float1x3 float1x4 float2x1 float2x2 float2x3 float2x4 float3x1 float3x2 float3x3 float3x4 float4x1 float4x2 float4x3 float4x4 ");
	m_HLSLKeyWords.Append("double1x1 double1x2 double1x3 double1x4 double2x1 double2x2 double2x3 double2x4 double3x1 double3x2 double3x3 double3x4 double4x1 double4x2 double4x3 double4x4 ");
	m_HLSLKeyWords.Append("bool1x1 bool1x2 bool1x3 bool1x4 bool2x1 bool2x2 bool2x3 bool2x4 bool3x1 bool3x2 bool3x3 bool3x4 bool4x1 bool4x2 bool4x3 bool4x4 sampler SamplerState texture ");
	m_HLSLKeyWords.Append("Texture2D Texture3D Texture1D TextureCUBE sampler1D sampler2D sampler3D samplerCUBE linear centroid nointerpolation noperspective sample typedef");

	// HLSL Funcs
	m_HLSLFuncs.Append("abort ""abs ""acos ""all ""AllMemoryBarrier ""AllMemoryBarrierWithGroupSync ""any ""asdouble ""asfloat ""asin ""asint ""asint ""asuint ""asuint ""atan ""atan2 ""ceil ""CheckAccessFullyMapped "
		"clamp ""clip ""cos ""cosh ""countbits ""cross ""D3DCOLORtoUBYTE4 ""ddx ""ddx_coarse ""ddx_fine ""ddy ""ddy_coarse ""ddy_fine ""degrees ""determinant ""DeviceMemoryBarrier ""DeviceMemoryBarrierWithGroupSync "
		"distance ""dot ""dst ""errorf ""EvaluateAttributeAtCentroid ""EvaluateAttributeAtSample ""EvaluateAttributeSnapped ""exp ""exp2 ""f16tof32 ""f32tof16 ""faceforward ""firstbithigh ""firstbitlow ""floor "
		"fma ""fmod ""frac ""frexp ""fwidth ""GetRenderTargetSampleCount ""GetRenderTargetSamplePosition ""GroupMemoryBarrier ""GroupMemoryBarrierWithGroupSync ""InterlockedAdd ""InterlockedAnd ""InterlockedCompareExchange "
		"InterlockedCompareStore ""InterlockedExchange ""InterlockedMax ""InterlockedMin ""InterlockedOr ""InterlockedXor ""isfinite ""isinf ""isnan ""ldexp ""length ""lerp ""lit ""log ""log10 ""log2 ""mad ""max ""min ""modf ""msad4 ""mul "
		"noise ""normalize ""pow ""printf ""Process2DQuadTessFactorsAvg ""Process2DQuadTessFactorsMax ""Process2DQuadTessFactorsMin ""ProcessIsolineTessFactors ""ProcessQuadTessFactorsAvg ""ProcessQuadTessFactorsMax ""ProcessQuadTessFactorsMin "
		"ProcessTriTessFactorsAvg ""ProcessTriTessFactorsMax ""ProcessTriTessFactorsMin ""radians ""rcp ""reflect ""refract ""reversebits ""round ""rsqrt ""saturate ""sign ""sin ""sincos ""sinh ""smoothstep ""sqrt ""step ""tan ""tanh ""tex1D "
		"tex1D ""tex1Dbias ""tex1Dgrad ""tex1Dlod ""tex1Dproj ""tex2D ""tex2D ""tex2Dbias ""tex2Dgrad ""tex2Dlod ""tex2Dproj ""tex3D ""tex3D ""tex3Dbias ""tex3Dgrad ""tex3Dlod ""tex3Dproj ""texCUBE ""texCUBE ""texCUBEbias ""texCUBEgrad ""texCUBElod "
		"texCUBEproj ""transpose ""trunc ""CalculateLevelOfDetail ""CalculateLevelOfDetailUnclamped ""Gather ""GetDimensions ""GetSamplePosition ""Load ""Sample ""SampleBias ""SampleCmp ""SampleCmpLevelZero ""SampleGrad ""SampleLevel");

	// Create a status bar on the bottom
	CreateStatusBar();

	CreateMainTextArea();
}

SavvyEditor::AppFrame::~AppFrame()
{

}

void SavvyEditor::AppFrame::OnExit(wxCommandEvent& a_Event)
{
	bool closed = Close(true);

	if (closed)
	{
		Destroy();
	}
}

void SavvyEditor::AppFrame::OnAbout(wxCommandEvent& a_Event)
{
	wxMessageBox("Savvy Editor\nVersion: 2.0\nAuthor: Apostol Dadachev",
		"About Savvy Editor", wxOK | wxICON_INFORMATION);
}

void SavvyEditor::AppFrame::OnFileOpen(wxCommandEvent& a_Event)
{
	wxFileDialog* openDialog = new wxFileDialog(this, "Open File~", "", "", "", wxFD_OPEN);
	int response = openDialog->ShowModal();

	// If everything went well, open the file
	if (response == wxID_OK) 
	{
		// Clear the Text Box
		m_TextAreaUser->ClearAll();
		m_TextAreaUser->LoadFile(openDialog->GetPath());
		m_CurrDocPath = openDialog->GetPath();
		SetTitle(openDialog->GetPath() + " - "DEFAULT_FRAME_TITLE);
	}
	openDialog->Destroy();
}

void SavvyEditor::AppFrame::OnFileNew(wxCommandEvent& a_Event)
{
	// Set the Title to reflect the file open
	SetTitle("Untitled* - "DEFAULT_FRAME_TITLE);
	m_TextAreaUser->ClearAll();
}

void SavvyEditor::AppFrame::OnFileSave(wxCommandEvent& a_Event)
{
	if (m_CurrDocPath != DEFAULT_DOC_PATH)
	{
		// Save to the already-set path for the document
		m_TextAreaUser->SaveFile(m_CurrDocPath);
		SetTitle(m_CurrDocPath + " - "DEFAULT_FRAME_TITLE);
	}
	else
	{
		// Fall-back if the file hasn't been saved before, use Save As
		OnFileSaveAs(a_Event);
	}
}

void SavvyEditor::AppFrame::OnFileSaveAs(wxCommandEvent& a_Event)
{
	wxFileDialog* saveDialog = new wxFileDialog(this, "Save File~", "", "", "Text Files (*.txt)|*.txt|C++ Files (*.cpp)|*.cpp|GLSL Files (*.glsl)|*.glsl|HLSL Files (*.hlsl)|*.hlsl", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	int response = saveDialog->ShowModal();

	// If everything went well, save the file
	if (response == wxID_OK)
	{
		m_TextAreaUser->SaveFile(saveDialog->GetPath());
		m_CurrDocPath = saveDialog->GetPath();
		SetTitle(m_CurrDocPath + " - "DEFAULT_FRAME_TITLE);
	}
	saveDialog->Destroy();
}

void SavvyEditor::AppFrame::OnFileClose(wxCommandEvent& a_Event)
{
	// Clear the Text Box
	m_TextAreaUser->ClearAll();
	// Reset the current File being edited
	m_CurrDocPath = DEFAULT_DOC_PATH;
	// Set the Title to reflect the file open
	SetTitle("Untitled* - "DEFAULT_FRAME_TITLE);
}

void SavvyEditor::AppFrame::CreateMainTextArea()
{
	// Create the text area
	wxSize areaSize = GetClientSize();
	areaSize.SetWidth(areaSize.GetWidth() / 2);
	m_TextAreaUser = new wxStyledTextCtrl(this, ID_TextAreaUser, wxDefaultPosition, areaSize, wxTE_PROCESS_ENTER | wxTE_MULTILINE);
	
	// Set up the margins for the line numbers
	m_TextAreaUser->StyleClearAll();
	m_TextAreaUser->SetLexer(wxSTC_LEX_CPP);

	m_TextAreaUser->SetMarginWidth(MARGIN_LINE_NUMBERS, MARGIN_LINE_NUMBERS_WIDTH);
	m_TextAreaUser->StyleSetForeground(wxSTC_STYLE_LINENUMBER, wxColour(75, 75, 75));
	m_TextAreaUser->StyleSetBackground(wxSTC_STYLE_LINENUMBER, wxColour(220, 220, 220));
	m_TextAreaUser->SetMarginType(MARGIN_LINE_NUMBERS, wxSTC_MARGIN_NUMBER);


	// ---- Enable code folding
	m_TextAreaUser->SetMarginType(MARGIN_FOLD, wxSTC_MARGIN_SYMBOL);
	m_TextAreaUser->SetMarginWidth(MARGIN_FOLD, MARGIN_FOLD_WIDTH);
	m_TextAreaUser->SetMarginMask(MARGIN_FOLD, wxSTC_MASK_FOLDERS);
	m_TextAreaUser->StyleSetBackground(MARGIN_FOLD, C_BLOCK_BACKGROUND_COLOR);
	m_TextAreaUser->SetMarginSensitive(MARGIN_FOLD, true);

	// Properties found from http://www.scintilla.org/SciTEDoc.html
	m_TextAreaUser->SetProperty(wxT("fold"), wxT("1"));
	m_TextAreaUser->SetProperty(wxT("fold.comment"), wxT("1"));
	m_TextAreaUser->SetProperty(wxT("fold.compact"), wxT("1"));

	m_TextAreaUser->MarkerDefine(wxSTC_MARKNUM_FOLDER, wxSTC_MARK_ARROW);
	m_TextAreaUser->MarkerSetForeground(wxSTC_MARKNUM_FOLDER, FOLDING_COLOR);
	m_TextAreaUser->MarkerSetBackground(wxSTC_MARKNUM_FOLDER, FOLDING_COLOR);

	m_TextAreaUser->MarkerDefine(wxSTC_MARKNUM_FOLDEROPEN, wxSTC_MARK_ARROWDOWN);
	m_TextAreaUser->MarkerSetForeground(wxSTC_MARKNUM_FOLDEROPEN, FOLDING_COLOR);
	m_TextAreaUser->MarkerSetBackground(wxSTC_MARKNUM_FOLDEROPEN, FOLDING_COLOR);

	m_TextAreaUser->MarkerDefine(wxSTC_MARKNUM_FOLDERSUB, wxSTC_MARK_EMPTY);
	m_TextAreaUser->MarkerSetForeground(wxSTC_MARKNUM_FOLDERSUB, FOLDING_COLOR);
	m_TextAreaUser->MarkerSetBackground(wxSTC_MARKNUM_FOLDERSUB, FOLDING_COLOR);

	m_TextAreaUser->MarkerDefine(wxSTC_MARKNUM_FOLDEREND, wxSTC_MARK_ARROW);
	m_TextAreaUser->MarkerSetForeground(wxSTC_MARKNUM_FOLDEREND, FOLDING_COLOR);
	m_TextAreaUser->MarkerSetBackground(wxSTC_MARKNUM_FOLDEREND, _T("WHITE"));

	m_TextAreaUser->MarkerDefine(wxSTC_MARKNUM_FOLDEROPENMID, wxSTC_MARK_ARROWDOWN);
	m_TextAreaUser->MarkerSetForeground(wxSTC_MARKNUM_FOLDEROPENMID, FOLDING_COLOR);
	m_TextAreaUser->MarkerSetBackground(wxSTC_MARKNUM_FOLDEROPENMID, _T("WHITE"));

	m_TextAreaUser->MarkerDefine(wxSTC_MARKNUM_FOLDERMIDTAIL, wxSTC_MARK_EMPTY);
	m_TextAreaUser->MarkerSetForeground(wxSTC_MARKNUM_FOLDERMIDTAIL, FOLDING_COLOR);
	m_TextAreaUser->MarkerSetBackground(wxSTC_MARKNUM_FOLDERMIDTAIL, FOLDING_COLOR);

	m_TextAreaUser->MarkerDefine(wxSTC_MARKNUM_FOLDERTAIL, wxSTC_MARK_EMPTY);
	m_TextAreaUser->MarkerSetForeground(wxSTC_MARKNUM_FOLDERTAIL, FOLDING_COLOR);
	m_TextAreaUser->MarkerSetBackground(wxSTC_MARKNUM_FOLDERTAIL, FOLDING_COLOR);
	// ---- End of code folding part

	m_TextAreaUser->SetWrapMode(wxSTC_WRAP_NONE);

	m_TextAreaUser->Connect(wxEVT_STC_MARGINCLICK, wxStyledTextEventHandler(SavvyEditor::AppFrame::OnMarginClick), NULL, this);

	SetTitle("Untitled* - "DEFAULT_FRAME_TITLE);
}

void SavvyEditor::AppFrame::OnResize(wxSizeEvent& a_Event)
{
	if (m_TextAreaUser)
	{
		wxSize areaSize = GetClientSize();
		areaSize.SetWidth(areaSize.GetWidth() / 2);
		m_TextAreaUser->SetSize(areaSize);
	}
}

void SavvyEditor::AppFrame::OnTextChanged(wxCommandEvent& event)
{
	if (m_CurrDocPath != DEFAULT_DOC_PATH)
	{
		SetTitle(m_CurrDocPath + "* - "DEFAULT_FRAME_TITLE);
	}
}

void SavvyEditor::AppFrame::OnUndo(wxCommandEvent& a_Event)
{
	m_TextAreaUser->Undo();
}

void SavvyEditor::AppFrame::OnRedo(wxCommandEvent& a_Event)
{
	m_TextAreaUser->Redo();
}

void SavvyEditor::AppFrame::OnCut(wxCommandEvent& a_Event)
{
	m_TextAreaUser->Cut();
}

void SavvyEditor::AppFrame::OnCopy(wxCommandEvent& a_Event)
{
	m_TextAreaUser->Copy();
}

void SavvyEditor::AppFrame::OnPaste(wxCommandEvent& a_Event)
{
	m_TextAreaUser->Paste();
}

void SavvyEditor::AppFrame::OnDelete(wxCommandEvent& a_Event)
{
	m_TextAreaUser->DeleteBack();
}

void SavvyEditor::AppFrame::OnSelectAll(wxCommandEvent& a_Event)
{
	m_TextAreaUser->SelectAll();
}

void SavvyEditor::AppFrame::OnConvert(wxCommandEvent& a_Event)
{
	wxMessageBox("TODO",
		"TODO", wxOK | wxICON_INFORMATION);
}

void SavvyEditor::AppFrame::OnMenuOpen(wxMenuEvent& a_Event)
{
	wxMenu* currMenu = a_Event.GetMenu();

	// Fixes a bug when clicking on the icon
	if (currMenu == NULL)
		return;

	wxMenuItem* currItem = NULL;
	if (currMenu->GetTitle() == "&Edit")
	{
		// Redo enable/disable
		if (m_TextAreaUser->CanRedo())
		{
			currItem = currMenu->FindItem(wxID_REDO);
			EnableMenuItem(currItem, true);
		}
		else
		{
			currItem = currMenu->FindItem(wxID_REDO);
			EnableMenuItem(currItem, false);
		}

		// Undo enable/disable
		if (m_TextAreaUser->CanUndo())
		{
			currItem = currMenu->FindItem(wxID_UNDO);
			EnableMenuItem(currItem, true);
		}
		else
		{
			currItem = currMenu->FindItem(wxID_UNDO);
			EnableMenuItem(currItem, false);
		}

		// Delete enable/disable
		if (!m_TextAreaUser->IsEmpty())
		{
			currItem = currMenu->FindItem(wxID_DELETE);
			EnableMenuItem(currItem, true);
		}
		else
		{
			currItem = currMenu->FindItem(wxID_DELETE);
			EnableMenuItem(currItem, false);
		}

		// Copy
		if (m_TextAreaUser->CanCopy())
		{
			currItem = currMenu->FindItem(wxID_COPY);
			EnableMenuItem(currItem, true);
		}
		else
		{
			currItem = currMenu->FindItem(wxID_COPY);
			EnableMenuItem(currItem, false);
		}

		// Cut
		if (m_TextAreaUser->CanCut())
		{
			currItem = currMenu->FindItem(wxID_CUT);
			EnableMenuItem(currItem, true);
		}
		else
		{
			currItem = currMenu->FindItem(wxID_CUT);
			EnableMenuItem(currItem, false);
		}

		// Paste
		if (m_TextAreaUser->CanPaste())
		{
			currItem = currMenu->FindItem(wxID_PASTE);
			EnableMenuItem(currItem, true);
		}
		else
		{
			currItem = currMenu->FindItem(wxID_PASTE);
			EnableMenuItem(currItem, false);
		}
	}
	else if (currMenu->GetTitle() == "&File")
	{
		if (m_TextAreaUser->IsModified())
		{
			currItem = currMenu->FindItem(wxID_SAVE);
			EnableMenuItem(currItem, true);
		}
		else
		{
			currItem = currMenu->FindItem(wxID_SAVE);
			EnableMenuItem(currItem, false);
		}
	}
}

void SavvyEditor::AppFrame::EnableMenuItem(wxMenuItem* a_Item, bool a_Enable)
{
	if (a_Item != NULL)
	{
		a_Item->Enable(a_Enable);
	}
}

// Event callback when a margin is clicked, used here for code folding
void SavvyEditor::AppFrame::OnMarginClick(wxStyledTextEvent& a_Event)
{
	if (a_Event.GetMargin() == MARGIN_FOLD)
	{
		int lineClick = m_TextAreaUser->LineFromPosition(a_Event.GetPosition());
		int levelClick = m_TextAreaUser->GetFoldLevel(lineClick);

		if ((levelClick & wxSTC_FOLDLEVELHEADERFLAG) > 0)
		{
			m_TextAreaUser->ToggleFold(lineClick);
		}
	}
}

void SavvyEditor::AppFrame::OnLangSelectGLSL(wxCommandEvent& a_Event)
{
	m_TextAreaUser->StyleSetForeground(wxSTC_C_STRING, C_STRING_COLOR);
	m_TextAreaUser->StyleSetForeground(wxSTC_C_PREPROCESSOR, C_PREPROCESSOR_COLOR);
	m_TextAreaUser->StyleSetForeground(wxSTC_C_IDENTIFIER, C_IDENTIFIER_COLOR);
	m_TextAreaUser->StyleSetForeground(wxSTC_C_NUMBER, C_NUMBER_COLOR);
	m_TextAreaUser->StyleSetForeground(wxSTC_C_CHARACTER, C_CHARACTER_COLOR);
	m_TextAreaUser->StyleSetForeground(wxSTC_C_WORD, C_WORD_COLOR);
	m_TextAreaUser->StyleSetForeground(wxSTC_C_WORD2, C_WORD2_COLOR);
	m_TextAreaUser->StyleSetForeground(wxSTC_C_COMMENT, C_COMMENT_COLOR);
	m_TextAreaUser->StyleSetForeground(wxSTC_C_COMMENTLINE, C_COMMENTLINE_COLOR);
	m_TextAreaUser->StyleSetForeground(wxSTC_C_COMMENTDOC, C_COMMENTDOC_COLOR);
	m_TextAreaUser->StyleSetForeground(wxSTC_C_COMMENTDOCKEYWORD, C_COMMENTDOCKEYWORD_COLOR);
	m_TextAreaUser->StyleSetForeground(wxSTC_C_COMMENTDOCKEYWORDERROR, C_COMMENTDOCKEYWORDERROR_COLOR);
	m_TextAreaUser->StyleSetBold(wxSTC_C_WORD, true);
	m_TextAreaUser->StyleSetBold(wxSTC_C_WORD2, true);
	m_TextAreaUser->StyleSetBold(wxSTC_C_COMMENTDOCKEYWORD, true);

	m_TextAreaUser->SetKeyWords(0, m_GLSLKeyWords);
	m_TextAreaUser->SetKeyWords(1, m_GLSLFuncs);
	
}

void SavvyEditor::AppFrame::OnLangSelectHLSL(wxCommandEvent& a_Event)
{
	m_TextAreaUser->StyleSetForeground(wxSTC_C_STRING, C_STRING_COLOR);
	m_TextAreaUser->StyleSetForeground(wxSTC_C_PREPROCESSOR, C_PREPROCESSOR_COLOR);
	m_TextAreaUser->StyleSetForeground(wxSTC_C_IDENTIFIER, C_IDENTIFIER_COLOR);
	m_TextAreaUser->StyleSetForeground(wxSTC_C_NUMBER, C_NUMBER_COLOR);
	m_TextAreaUser->StyleSetForeground(wxSTC_C_CHARACTER, C_CHARACTER_COLOR);
	m_TextAreaUser->StyleSetForeground(wxSTC_C_WORD, C_WORD_COLOR);
	m_TextAreaUser->StyleSetForeground(wxSTC_C_WORD2, C_WORD2_COLOR);
	m_TextAreaUser->StyleSetForeground(wxSTC_C_COMMENT, C_COMMENT_COLOR);
	m_TextAreaUser->StyleSetForeground(wxSTC_C_COMMENTLINE, C_COMMENTLINE_COLOR);
	m_TextAreaUser->StyleSetForeground(wxSTC_C_COMMENTDOC, C_COMMENTDOC_COLOR);
	m_TextAreaUser->StyleSetForeground(wxSTC_C_COMMENTDOCKEYWORD, C_COMMENTDOCKEYWORD_COLOR);
	m_TextAreaUser->StyleSetForeground(wxSTC_C_COMMENTDOCKEYWORDERROR, C_COMMENTDOCKEYWORDERROR_COLOR);
	m_TextAreaUser->StyleSetBold(wxSTC_C_WORD, true);
	m_TextAreaUser->StyleSetBold(wxSTC_C_WORD2, true);
	m_TextAreaUser->StyleSetBold(wxSTC_C_COMMENTDOCKEYWORD, true);

	m_TextAreaUser->SetKeyWords(0, m_HLSLKeyWords);
	m_TextAreaUser->SetKeyWords(1, m_HLSLFuncs);
}

void SavvyEditor::AppFrame::OnLangSelectNone(wxCommandEvent& a_Event)
{
	m_TextAreaUser->StyleClearAll();
	m_TextAreaUser->SetLexer(wxSTC_LEX_CPP);

	m_TextAreaUser->SetMarginWidth(MARGIN_LINE_NUMBERS, MARGIN_LINE_NUMBERS_WIDTH);
	m_TextAreaUser->StyleSetForeground(wxSTC_STYLE_LINENUMBER, wxColour(75, 75, 75));
	m_TextAreaUser->StyleSetBackground(wxSTC_STYLE_LINENUMBER, wxColour(220, 220, 220));
	m_TextAreaUser->SetMarginType(MARGIN_LINE_NUMBERS, wxSTC_MARGIN_NUMBER);


	// ---- Enable code folding
	m_TextAreaUser->SetMarginType(MARGIN_FOLD, wxSTC_MARGIN_SYMBOL);
	m_TextAreaUser->SetMarginWidth(MARGIN_FOLD, MARGIN_FOLD_WIDTH);
	m_TextAreaUser->SetMarginMask(MARGIN_FOLD, wxSTC_MASK_FOLDERS);
	m_TextAreaUser->StyleSetBackground(MARGIN_FOLD, C_BLOCK_BACKGROUND_COLOR);
	m_TextAreaUser->SetMarginSensitive(MARGIN_FOLD, true);

	// Properties found from http://www.scintilla.org/SciTEDoc.html
	m_TextAreaUser->SetProperty(wxT("fold"), wxT("1"));
	m_TextAreaUser->SetProperty(wxT("fold.comment"), wxT("1"));
	m_TextAreaUser->SetProperty(wxT("fold.compact"), wxT("1"));

	m_TextAreaUser->MarkerDefine(wxSTC_MARKNUM_FOLDER, wxSTC_MARK_ARROW);
	m_TextAreaUser->MarkerSetForeground(wxSTC_MARKNUM_FOLDER, FOLDING_COLOR);
	m_TextAreaUser->MarkerSetBackground(wxSTC_MARKNUM_FOLDER, FOLDING_COLOR);

	m_TextAreaUser->MarkerDefine(wxSTC_MARKNUM_FOLDEROPEN, wxSTC_MARK_ARROWDOWN);
	m_TextAreaUser->MarkerSetForeground(wxSTC_MARKNUM_FOLDEROPEN, FOLDING_COLOR);
	m_TextAreaUser->MarkerSetBackground(wxSTC_MARKNUM_FOLDEROPEN, FOLDING_COLOR);

	m_TextAreaUser->MarkerDefine(wxSTC_MARKNUM_FOLDERSUB, wxSTC_MARK_EMPTY);
	m_TextAreaUser->MarkerSetForeground(wxSTC_MARKNUM_FOLDERSUB, FOLDING_COLOR);
	m_TextAreaUser->MarkerSetBackground(wxSTC_MARKNUM_FOLDERSUB, FOLDING_COLOR);

	m_TextAreaUser->MarkerDefine(wxSTC_MARKNUM_FOLDEREND, wxSTC_MARK_ARROW);
	m_TextAreaUser->MarkerSetForeground(wxSTC_MARKNUM_FOLDEREND, FOLDING_COLOR);
	m_TextAreaUser->MarkerSetBackground(wxSTC_MARKNUM_FOLDEREND, _T("WHITE"));

	m_TextAreaUser->MarkerDefine(wxSTC_MARKNUM_FOLDEROPENMID, wxSTC_MARK_ARROWDOWN);
	m_TextAreaUser->MarkerSetForeground(wxSTC_MARKNUM_FOLDEROPENMID, FOLDING_COLOR);
	m_TextAreaUser->MarkerSetBackground(wxSTC_MARKNUM_FOLDEROPENMID, _T("WHITE"));

	m_TextAreaUser->MarkerDefine(wxSTC_MARKNUM_FOLDERMIDTAIL, wxSTC_MARK_EMPTY);
	m_TextAreaUser->MarkerSetForeground(wxSTC_MARKNUM_FOLDERMIDTAIL, FOLDING_COLOR);
	m_TextAreaUser->MarkerSetBackground(wxSTC_MARKNUM_FOLDERMIDTAIL, FOLDING_COLOR);

	m_TextAreaUser->MarkerDefine(wxSTC_MARKNUM_FOLDERTAIL, wxSTC_MARK_EMPTY);
	m_TextAreaUser->MarkerSetForeground(wxSTC_MARKNUM_FOLDERTAIL, FOLDING_COLOR);
	m_TextAreaUser->MarkerSetBackground(wxSTC_MARKNUM_FOLDERTAIL, FOLDING_COLOR);
	// ---- End of code folding part

	m_TextAreaUser->SetWrapMode(wxSTC_WRAP_NONE);
}

void SavvyEditor::AppFrame::OnShowReplaceDialog(wxCommandEvent& a_Event)
{
	if (m_ReplaceDialog)
	{
		wxDELETE(m_ReplaceDialog);
	}
	else
	{
		m_ReplaceDialog = new wxFindReplaceDialog(this, &m_FindData, wxT("Find and replace dialog"), wxFR_REPLACEDIALOG);

		m_ReplaceDialog->Show(true);
	}
}

void SavvyEditor::AppFrame::OnShowFindDialog(wxCommandEvent& a_event)
{
	if (m_FindDialog)
	{
		wxDELETE(m_FindDialog);
	}
	else
	{
		m_FindDialog = new wxFindReplaceDialog(this, &m_FindData, wxT("Find dialog"));

		m_FindDialog->Show(true);
	}
}

void SavvyEditor::AppFrame::OnFindDialog(wxFindDialogEvent& a_Event)
{
	wxEventType type = a_Event.GetEventType();

	if (type == wxEVT_FIND || type == wxEVT_FIND_NEXT)
	{
		if (!DoFind(a_Event.GetFindString(), a_Event.GetFlags()))
		{
			wxMessageBox(wxT("No more matches."), DEFAULT_FRAME_TITLE);
		}
	}
	else if (type == wxEVT_FIND_REPLACE)
	{
		if (!DoReplace(a_Event.GetFindString(), a_Event.GetReplaceString(), a_Event.GetFlags()))
		{
			wxMessageBox(wxT("Nothing to replace."), DEFAULT_FRAME_TITLE);
		}
	}
	else if (type == wxEVT_FIND_REPLACE_ALL)
	{
		int numReplaced = DoReplaceAll(a_Event.GetFindString(), a_Event.GetReplaceString(), a_Event.GetFlags());
		if (numReplaced > 0)
		{
			wxString numString = wxString::Format(wxT("%i"), numReplaced);
			numString.Append(" occurrences replaced.");
			wxMessageBox(numString, DEFAULT_FRAME_TITLE);
		}
		else
		{
			wxMessageBox(wxT("Nothing to replace."), DEFAULT_FRAME_TITLE);
		}
	}
	else if (type == wxEVT_FIND_CLOSE)
	{
		wxFindReplaceDialog *dlg = a_Event.GetDialog();

		int idMenu;
		const wxChar *txt;
		if (dlg == m_FindDialog)
		{
			txt = wxT("Find");
			idMenu = ID_FindDialog;
			m_FindDialog = NULL;
		}
		else if (dlg == m_ReplaceDialog)
		{
			txt = wxT("Replace");
			idMenu = ID_ReplaceDialog;
			m_ReplaceDialog = NULL;
		}
		else
		{
			txt = wxT("Unknown");
			idMenu = wxID_ANY;

			wxFAIL_MSG(wxT("unexpected event"));
		}

		if (idMenu != wxID_ANY)
		{
			GetMenuBar()->Check(idMenu, false);
		}

		dlg->Destroy();
	}
	else
	{
		wxLogError(wxT("Unknown find dialog event!"));
	}
}

bool SavvyEditor::AppFrame::DoFind(wxString& a_FindString, int a_Flags)
{
	m_TextAreaUser->SetCurrentPos(m_CurrFindPos + 1);
	m_TextAreaUser->SearchAnchor();
	int lastLine = m_TextAreaUser->GetCurrentLine();

	if (a_Flags & wxFR_DOWN)
	{
		m_CurrFindPos = m_TextAreaUser->SearchNext(a_Flags, a_FindString);
	}
	else
	{
		m_CurrFindPos = m_TextAreaUser->SearchPrev(a_Flags, a_FindString);
	}
	
	if (m_CurrFindPos == -1)
	{
		// Not found
		m_CurrFindPos = 0;
		return false;
	}
	int newLine = m_TextAreaUser->GetCurrentLine();
	m_TextAreaUser->Freeze();
	m_TextAreaUser->ScrollLines(newLine - lastLine);
	m_TextAreaUser->Thaw();
	return true;
}

bool SavvyEditor::AppFrame::DoReplace(wxString& a_FindString, const wxString& a_ReplaceString, int a_Flags)
{
	m_TextAreaUser->SetCurrentPos(m_CurrFindPos + 1);
	m_TextAreaUser->SearchAnchor();
	int lastLine = m_TextAreaUser->GetCurrentLine();
	
	if (a_Flags & wxFR_DOWN)
	{
		m_CurrFindPos = m_TextAreaUser->SearchNext(a_Flags, a_FindString);
	}
	else
	{
		m_CurrFindPos = m_TextAreaUser->SearchPrev(a_Flags, a_FindString);
	}

	if (m_CurrFindPos == -1)
	{
		// Not found
		m_CurrFindPos = 0;
		return false;
	}
	int newLine = m_TextAreaUser->GetCurrentLine();
	m_TextAreaUser->Freeze();
	m_TextAreaUser->ScrollLines(newLine - lastLine);

	// Replace
	m_TextAreaUser->RemoveSelection();
	m_TextAreaUser->AddText(a_ReplaceString);

	m_TextAreaUser->Thaw();
	return true;
}

int SavvyEditor::AppFrame::DoReplaceAll(wxString& a_FindString, const wxString& a_ReplaceString, int a_Flags)
{
	int ctrReplaced = 0;

	while (true)
	{
		m_TextAreaUser->SetCurrentPos(m_CurrFindPos + 1);
		m_TextAreaUser->SearchAnchor();
		int lastLine = m_TextAreaUser->GetCurrentLine();

		if (a_Flags & wxFR_DOWN)
		{
			m_CurrFindPos = m_TextAreaUser->SearchNext(a_Flags, a_FindString);
		}
		else
		{
			m_CurrFindPos = m_TextAreaUser->SearchPrev(a_Flags, a_FindString);
		}

		if (m_CurrFindPos == -1)
		{
			// Not found
			m_CurrFindPos = 0;
			break;
		}
		m_TextAreaUser->Freeze();

		// Replace, no scroll
		m_TextAreaUser->RemoveSelection();
		m_TextAreaUser->AddText(a_ReplaceString);

		m_TextAreaUser->Thaw();
		ctrReplaced++;
	}

	return ctrReplaced;
}
