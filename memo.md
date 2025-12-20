# Step 1
* 幅優先探索で自然と探索順序はlevel orderになる
* 問題はどうやってreturnするリストを構築するか．
* 以下の方針で書いたのが[Code1](#Code1)
  * 幅優先探索に使うキューに，ノードとその深さのペアを入れておく
  * 直前のループで扱ったノードと，今扱っているノードの深さが違ったら，levelが変わったと判定できる
  * levelが変わったらそのlevel用の空のvector<int>を，return用のvector<vector<int>>にpush_back
  * ノードの探索が終了したらそのノードのlevelに対応するvector<int>にnode->valをpush_back
### Code1
```cpp
class Solution {
public:
    vector<vector<int>> levelOrder(TreeNode* root) {
        vector<vector<int>> nodes_list;
        if (!root) {
            return nodes_list;
        }

        deque<pair<TreeNode*, int>> uncompleted_points; // 各成分は{node, depth}
        uncompleted_points.push_back({root, 1});
        
        int previous_depth = 0;
        while (!uncompleted_points.empty()) {
            auto [current_node, current_depth] = uncompleted_points.front();
            uncompleted_points.pop_front();
            
            if (current_depth != previous_depth) {
                nodes_list.push_back({});
            }

            if (current_node->left) {
                uncompleted_points.push_back({current_node->left, current_depth + 1});
            }
            if (current_node->right) {
                uncompleted_points.push_back({current_node->right, current_depth + 1});
            }

            nodes_list[current_depth - 1].push_back(current_node->val);
            previous_depth = current_depth;
        }

        return nodes_list;
    }
};
```
# Step 2
* Step1から時間が空いたので解き直してみる．[Code2](#Code2)．所要時間17分
  * キューを2つ用意すればStep1のようにdepthを保持する必要なく素直にlevel order traversalができる．
  * キューに入れるのは`TreeNode*`だけど出力の`vector`に入っているのは`int`なので注意．単純にコンストラクタ呼び出しで構築できないので別に関数を用意した．
  * また，出力にnullptrは入れないので「とりあえずキューに追加してから処理の直前でnullptrを弾く」はできない
    * と思ったけどキューから出力用ベクターを得る関数を自前で作ってるから別にできる．
    * と思ったけど`next_frontiers`にnullptrだけ入っていたら最終出力に`[]`が追加される．これ弾くにはいちいち出力用ベクターに追加する前に，frontierの中身を舐めるチェックが必要でめんどくさそう．

### Code2
```cpp
class Solution {
public:
    vector<vector<int>> levelOrder(TreeNode* root) {
        vector<vector<int>> result;
        if (!root) {
            return result;
        }
        std::deque<TreeNode*> frontiers;
        std::deque<TreeNode*> next_frontiers;
        next_frontiers.push_back(root);
        while (!next_frontiers.empty()) {
            frontiers = std::move(next_frontiers);
            next_frontiers.clear();
            result.push_back(GetValsFromNodes(frontiers));
            
            while (!frontiers.empty()) {
                TreeNode* node = frontiers.front();
                frontiers.pop_front();
                for (TreeNode* child : {node->left, node->right}) {
                    if (child) {
                        next_frontiers.push_back(child);
                    }
                }
            }
        }
        return result;
    }
private:
    vector<int> GetValsFromNodes(const std::deque<TreeNode*> nodes) {
        vector<int> result;
        for (TreeNode* node : nodes) {
            result.push_back(node->val);
        }
        return result;
    }
};
```
