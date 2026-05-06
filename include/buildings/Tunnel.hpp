//
// Created by joshb on 2026/3/23.
//

#ifndef REPLACE_WITH_YOUR_PROJECT_NAME_TUNNEL_HPP
#define REPLACE_WITH_YOUR_PROJECT_NAME_TUNNEL_HPP
#include "Machine.hpp"
#include "ItemAcceptor.hpp"
#include "ItemEjector.hpp"
#include "Util/Image.hpp"
#include "Opti/OptiImage.hpp"

enum class TunnelType {
    IN, OUT
};

class Tunnel: public Machine {
public:
    static inline std::vector<std::shared_ptr<OptiImage>> tunnelInTextures, tunnelOutTextures;
    std::shared_ptr<Tunnel> other;
    std::shared_ptr<ItemAcceptor> acceptor;
    std::shared_ptr<ItemEjector> ejector;
    TunnelType type;
    int distance; // distance between two tunnels
    bool upgraded;
    Tunnel(int x, int y, int r, TunnelType type, bool upgraded);
    std::string getSaveString();
    static std::shared_ptr<Machine> fromSaveString(std::vector<std::string> prop);
    void Init();
    void Update();
    void Delete();
    void Restore(int arg);
    void Promote();
    void Pair();
};

#endif //REPLACE_WITH_YOUR_PROJECT_NAME_TUNNEL_HPP