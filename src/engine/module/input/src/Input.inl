
namespace mono::input
{

bool arePressed(std::same_as<std::int32_t> auto... keys)
{
    if constexpr(config::constant::debugBuild)
    {
        priv::ensureGlfwIsInitialized();
    }
    (priv::updateKeyState(keys), ...);
    return (
        (priv::isKeyInState(keys, KeyState::HOLD) or priv::isKeyInState(keys, KeyState::PRESS_ONCE))
        and ...);
}

}  // namespace mono::input
