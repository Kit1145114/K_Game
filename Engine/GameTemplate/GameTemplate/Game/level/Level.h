#pragma once


class MapChip;
/*!
* @brief	オブジェクト名。
*/
struct LevelObjectData {
	CVector3 position;		//<座標。
	CQuaternion rotation;	//!<回転。
	CVector3 scale;			//スケール
	const wchar_t* name;	//!<名前。
	/// <summary>
	/// 引数で渡したオブジェクト名を調べる
	/// </summary>
	/// <param name="objName">調べる名前</param>
	/// <returns>名前が同じならtrueを返す。</returns>
	bool EqualObjectName(const wchar_t* objName)
	{
		return wcscmp(objName, name) == 0;
	}
};
/*!
 * @brief	レベル
 *@
 */
class Level {
public :
	Level();
	~Level();

	/// <summary>
	/// 引数で渡したオブジェクト名を調べる
	/// </summary>
	/// <param name="objName">調べる名前</param>
	/// <returns>名前が同じならtrueを返す。</returns>
	bool EqualObjectName(const wchar_t* objName)
	{
		return wcscmp(objName, levelobjData.name) == 0;
	}
	/*!
	* @brief	マップチップのスマートポインタの別名定義。
	*@details
	* std::unique_ptrは渡されたリソースの所有権を唯一(ユニーク)持っているように
	* 振舞うスマートポインタ。動的に確保されたリソースは、インスタンスの寿命が
	* 尽きると自動的に破棄されます。
	*/
	using MapChipPtr = std::unique_ptr<MapChip>;
	/*!
	* @brief	オブジェクトをビルドする時にフックする関数オブジェクトの型の別名定義。
	*@details
	* 引数にはオブジェクトの情報が渡されます。
	* この関数がfalseを返すと、オブジェクトの情報から、静的オブジェクトのMapChipクラスのインスタンスが
	* 生成されます。
	* オブジェクトの名前などで、ドアやジャンプ台、アイテムなどの特殊なクラスのインスタンスを生成したときに、
	* デフォルトで作成されるMapChipクラスのインスタンスが不要な場合はtrueを返してください。
	* 例えば、フック関数の中で、渡されたオブジェクトデータの名前のモデルを描画するクラスのインスタンスを
	* 生成したときに、falseを返してしまうと、同じモデルが二つ描画されることになります。

	*/
	using HookWhenBuildObjectFunc = std::function<bool(LevelObjectData& objData)>;
	/*!
	* @brief	レベルを初期化。
	*@param[in]	levelDataFilePath		tklファイルのファイルパス。
	*@param[in] hookFunc				オブジェクトを作成する時の処理をフックするための関数オブジェクト。
	*									フックしないならnullptrを指定すればよい、
	*									詳細はHookWhenBuildObjectFuncのコメントを参照。
	*/
	void Init(const wchar_t* levelDataFilePath, HookWhenBuildObjectFunc hookFunc);
	//void Init(const wchar_t* levelDataFilePath, std::function<bool(LevelObjectData& objData)>hookFunc);
	/*!
	* @brief	レベルを描画。
	*/
	void Draw();
private:
	std::vector<MapChipPtr> m_mapChipArray;//!<マップチップの可変長配列。
	LevelObjectData levelobjData;
};