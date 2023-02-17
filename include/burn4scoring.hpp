#include <eosio/eosio.hpp>
#include <atomicassets-interface.hpp>

using namespace eosio;

CONTRACT burn4scoring : public contract {
    public:
        using contract::contract;

        ACTION upsertconf(
            uint32_t range,
            uint32_t score
        );

        ACTION remconf(
            uint32_t range
        );

        ACTION addallow(
            name collection
        );

        ACTION remallow(
            name collection
        );

        ACTION addexclude(
            name wallet
        );

        ACTION remexclude(
            name wallet
        );

        ACTION upsertscore(
            name wallet,
            uint32_t score
        );

        ACTION remscore(
            name wallet
        );

        ACTION dropscore();

        ACTION upsertredeem(
            int32_t template_id,
            std::vector<extended_asset> quantities
        );

        ACTION remredeem(
            int32_t template_id
        );

        [[eosio::on_notify("atomicassets::logburnasset")]] void redeem(
            name asset_owner,
            uint64_t asset_id,
            name collection_name,
            name schema_name,
            int32_t template_id,
            std::vector<asset> backed_tokens,
            atomicassets::ATTRIBUTE_MAP old_immutable_data,
            atomicassets::ATTRIBUTE_MAP old_mutable_data,
            name asset_ram_payer
        );

    private:
        TABLE config_entity {
            uint32_t range;
            uint32_t score;

            auto primary_key() const { return range; }
        };
        typedef multi_index<name("config"), config_entity> config_t;

        TABLE allowed_entity {
            name collection;

            auto primary_key() const { return collection.value; }
        };
        typedef multi_index<name("allowed"), allowed_entity> allowed_t;

        TABLE excluded_entity {
            name wallet;

            auto primary_key() const { return wallet.value; }
        };
        typedef multi_index<name("excluded"), excluded_entity> excluded_t;

        TABLE score_entity {
            name wallet;
            uint32_t score;

            auto primary_key() const { return wallet.value; }
        };
        typedef multi_index<name("score"), score_entity> score_t;

        TABLE redeem_entity {
            int32_t template_id;
            std::vector<extended_asset> quantities;

            auto primary_key() const { return template_id; }
        };
        typedef multi_index<name("redeem"), redeem_entity> redeem_t;

        void transfer_token(
            name receiver,
            name contract,
            asset amount,
            std::string memo
        );
};
