/*!
 * \author ddubois 
 * \date 26-Feb-18.
 */

#ifndef RENDERER_OPEN_GL_CONTEXT_H
#define RENDERER_OPEN_GL_CONTEXT_H

#include "objects/textures/texture2D.h"

namespace nova {
    /*!
     * \brief Manages OpenGL state by emulating the OpenGL state machine and only issuing OpenGL calls to change things
     * when they actually change. Meant as a layer between the render loop and the OpenGL context so that we don't
     * constantly bind texture x to texture unit y or something equally silly
     *
     * This class exposes a number of functions which mimic the setters for OpenGL state, such as methods to bind
     * textures, UBOs, shaders, etc. to the OpenGL context. This class will check to see if that bit of data is already
     * bound. If so, awesome! No ned to do anything else. If and only if the data is not bound will a GL call be issued
     */
    class open_gl_context {
    public:
        /*!
         * \brief Sets the default values for all the rendering states
         *
         * Blending: Off
         * Culling: On, cull backfaces
         * Depth Write: On
         * Depth Test: On
         * Stencil Test: Off
         * Stencil Write: Off
         * Color Write: On
         * Alpha to Coverage: Off
         * Alpha Write: On
         */
        void set_default_state();

        /*!
         * \brief Sets blending to be either enables or disabled
         * \param enabled If true, enables blending. If false, does not
         */
        void set_blending_enabled(bool enabled);

        /*!
         * \brief Sets face culling to be either enabled or disabled
         * \param enabled If true, enables face culling. If false, disables it
         */
        void set_culling_enabled(bool enabled);

        /*!
         * \brief Sets the face culling mode
         * \param culling_mode Either GL_BACK or GL_FRONT, depending on which face you want to cull
         */
        void set_culling_mode(int culling_mode);

        /*!
         * \brief Sets whether depth writing is enabled or not
         * \param enabled If true, depth writing is enabled. If false, it is not
         */
        void set_depth_write_enabled(bool enabled);

        /*!
         * \brief Sets whether the depth test is enabled or not
         * \param enabled If true, depth test is enabled. If false, it it not
         */
        void set_depth_test_enabled(bool enabled);

        /*!
         * \brief Sets whether the stencil test is enabled or not
         * \param enabled If true, the stencil test is enabled. If false, it is not
         */
        void set_stencil_test_enabled(bool enabled);

        /*!
         * \brief Sets whether writing to the stencil buffer is enabled or not
         * \param enabled If true, stencil writing is enabled. If false, it is not
         */
        void set_stencil_write_enabled(bool enabled);

        /*!
         * \brief Sets whether writing to the color buffer is enabled or not
         * \param enabled If true, color writes are enabled. If false, they are not
         */
        void set_color_write_enabled(bool enabled);

        /*!
         * \brief Sets whether alpha to coverage is enabled or not
         * \param enabled If true, alpha-to-coverage is enabled. If false, it is not
         */
        void set_alpha_to_coverage_enabled(bool enabled);

        /*!
         * \brief Sets whether alpha to coverage is enabled or not
         * \param enabled If true, alpha to coverage is enabled. If false, it is not
         */
        void set_alpha_write_enabled(bool enabled);

        /*!
         * \brief Set the stencil ops for a face group
         *
         * See https://www.khronos.org/opengl/wiki/GLAPI/glStencilOpSeparate for more docs
         *
         * \param face Whether to set this for front faces, back faces, or both
         * \param fail_op What to do when the stencil test fails
         * \param pass_op What to do when the stencil test passes but the depth test fails
         * \param depth_fail_op What to do when the stencil text fails and the depth test fails;
         */
        void set_stencil_op_separate(int face, int fail_op, int pass_op, int depth_fail_op);

        /*!
         * \brief Set the stencil test parameters for a face group
         *
         * See https://www.khronos.org/opengl/wiki/GLAPI/glStencilFuncSeparate for more docs
         *
         * \param face Whether to set this for the front faces, back faces, or both
         * \param compare_op The operation to use to perform the stencil test
         * \param reference The reference value to compare the stencil buffer value against
         * \param mask A bitwise mask to apply to both the stencil buffer value and the reference value
         */
        void set_stencil_func_separate(int face, int compare_op, int reference, int mask);

        /*!
         * \brief Binds a texture to a texture unit
         *
         * \param texture The texture to bind
         * \param texture_unit The texture unit to bind it to
         */
        void bind_texture(const texture2D &texture, const uint32_t texture_unit);
    };
}

#endif //RENDERER_OPEN_GL_CONTEXT_H
