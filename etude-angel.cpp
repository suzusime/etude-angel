#include <iostream>
#include <cassert>

#include <angelscript.h>
#include <scriptstdstring/scriptstdstring.h>
#include <scriptbuilder/scriptbuilder.h>

// スクリプト中から呼ぶヘルパー函数
void print(std::string &str)
{
    std::cout << str;
}

// メッセージコールバック函数
// コンパイルエラー等のメッセージはここで出力される
void MessageCallback(const asSMessageInfo *msg, void *param)
{
	const char *type = "ERR ";
	if( msg->type == asMSGTYPE_WARNING ) 
		type = "WARN";
	else if( msg->type == asMSGTYPE_INFORMATION ) 
		type = "INFO";

	printf("%s (%d, %d) : %s : %s\n", msg->section, msg->row, msg->col, type, msg->message);
}

int RunApplication(){
    using namespace std;
    int r = 0; //エラーコード等の返値を入れる

    // --- 準備部 --- //
    // スクリプトエンジンの作成
	asIScriptEngine *engine = asCreateScriptEngine();
	if( engine == 0 )
	{
		cout << "スクリプトエンジンの作成に失敗" << endl;
		return -1;
	}
    
    // The script compiler will write any compiler messages to the callback.
	engine->SetMessageCallback(asFUNCTION(MessageCallback), 0, asCALL_CDECL);

    // string アドオンの登録
    RegisterStdString(engine);

    // print函数をスクリプトエンジンにグローバル函数として登録
    r = engine->RegisterGlobalFunction("void print(const string &in)", asFUNCTION(print), asCALL_CDECL); assert(r>=0);

    // スクリプトビルダーの作成
    CScriptBuilder builder;

    // モジュールの登録
    builder.StartNewModule(engine, "Hello");

    // スクリプトファイルの読み込み
    if( builder.AddSectionFromFile("hello.as") < 0){
        printf("スクリプトファイルの読み込みに失敗");
    }
    
    // スクリプトのビルド
    builder.BuildModule();

    // --- 実行部 ---//
    // モジュールの取得
    asIScriptModule* mod = engine->GetModule("Hello");

    // 函数の取得
    asIScriptFunction* func = mod->GetFunctionByDecl("void main()");

    // 実行文脈の作成
    asIScriptContext *ctx = engine->CreateContext();
    
    //実行
    for(int i=0; i<5; ++i){
        ctx->Prepare(func);
        ctx->Execute();
    }

    // コンテキストの解放
    ctx->Release();

    // スクリプトエンジンの終了
	engine->ShutDownAndRelease();
}

// エントリポイント
int main(){
    RunApplication();
    return 0;
}

