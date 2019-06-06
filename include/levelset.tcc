//! Constructor with zero level set id and coefficient vector
template <unsigned Tdim>
mpm::LevelSet<Tdim>::LevelSet(unsigned id, const std::string& domain,
                              bool moving_statuss)
    : id_{id}, moving_{moving_statuss} {
  //! Logger
  std::string logger =
      "levelset" + std::to_string(Tdim) + "d::" + std::to_string(id);
  console_ = std::make_unique<spdlog::logger>(logger, mpm::stdout_sink);

  try {
    if (domain == "Integration_In")
      integration_domain_ = mpm::IntegrationDomain::Integration_In;
    else if (domain == "Integration_Out")
      integration_domain_ = mpm::IntegrationDomain::Integration_Out;
    else if (domain == "Boundary")
      integration_domain_ = mpm::IntegrationDomain::Boundary;
    else
      throw std::runtime_error(
          "Integration domain for level set is not properlyspecified, using "
          "default domain-in");
  } catch (std::exception& exception) {
    console_->warn("{} #{}: {}", __FILE__, __LINE__, exception.what());
  }
}
