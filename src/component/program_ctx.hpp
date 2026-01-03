#pragma once

namespace cent::component {

/**
 * \brief Read only view of the program context
 *
 * Used by drivers to
 * */
class ProgramCtxView {
 public:
    virtual ~ProgramCtxView() = default;
};

/**
 * \brief Provides information about the program
 *
 * Used by the drivers to get information about how cent is executed and to
 * access different "services".
 * */
class ProgramCtx {
 public:
};

}  // namespace cent::component
