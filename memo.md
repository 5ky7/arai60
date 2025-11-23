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
