/**
 * @file singleton_manager.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-02-15
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once
#include <pch.hpp>

#include <functional>
#include <memory>
#include <mutex>
#include <thread>
#include <unordered_map>


namespace is
{
    // mozc式シングルトン
    // https://qiita.com/kikuuuty/items/fcf5f7df2f0493c437dc
    class IS_WIN_API SingletonManager
    {
    public:
        /**
         * @brief SINGLETON型のシングルトンクラスを作成
         * SINGLETON型は必ず以下のルールを遵守すること.
         * 1) コンストラクタはprivate
         * 2) デストラクタはpublic
         * 3) SingletonManagerをfriendに登録
         *
         * @tparam SINGLETON
         * @return SINGLETON*
         */
        template <typename SINGLETON>
        static SINGLETON *Get();

        /**
         * @brief シングルトンクラスを識別するIDを取得
         *
         * @tparam SINGLETON
         * @return int
         */
        template <typename SINGLETON>
        static int GetId();

        /**
         * @brief 登録済みのすべてのシングルトンインスタンスを削除
         *
         */
        static void Clear();

        /**
         * @brief SINGLETON型の登録済みシングルトンインスタンスを削除
         *
         * @tparam SINGLETON
         */
        template <typename SINGLETON>
        static void Erase();

        /**
         * @brief IDによる登録済みシングルトンインスタンスの削除
         *
         * @param id
         */
        static void EraseById(int id);

    private:
        int mCount{0}; // 登録されたシングルトンインスタンスの数

        // [instance-ID], (addressとdeleter関数)]へのHashマップ
        std::unordered_map<int, std::pair<uintptr_t, std::function<void()>>> mSingletons;

        // addressからIDへのHashマップ
        std::unordered_map<uintptr_t, int> mAddress2Id;

        /**
         * @brief シングルトンインスタンスへのポインタ
         * https://qiita.com/Chironian/items/3fb61cffa2a20dbee5c2
         */
        static SingletonManager *sSelf; // Never be destroyed.

        /**
         * @brief このシングルトンインスタンスを取得
         * @warning シングルトンインスタンスの生成をスレッドセーフにするために
         *          必ずstd::mutexを定義しているSingletonManager::get()内部から
         *          呼び出すように制限をかけるからprivateスコープにする(get_rmtx関数を定義すれば大丈夫かも...)
         * @return SingletonManager&
         */
        static SingletonManager &GetSelf();

        /**
         * @brief ロック可能なミューテックスを取得
         * @note 追加 Shinichi Inoue 21/2/5
         * @return std::mutex&
         */
        static std::mutex &GetMutex();

        // コンストラクタ/デストラクタを非公開
        SingletonManager();
        ~SingletonManager();

        // コピー系オペレータをprivateに変更
        IS_DISABLE_COPY_AND_ASSIGN(SingletonManager)
    };
}