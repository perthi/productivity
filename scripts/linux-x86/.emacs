;; .emacs

;;; uncomment this line to disable loading of "default.el" at startup
;; (setq inhibit-default-init t)
;;(set-foreground-color "blanched almond")	
;;(set-foreground-color "white")
(global-set-key [f12] 'compile)
(set-foreground-color "#f2f2f2")
;;(set-background-color "black")
(set-background-color "#001A16")
;;(set-background-color "#00332B")
;; turn on font-lock mode
(when (fboundp 'global-font-lock-mode)
  (global-font-lock-mode t))

  ;(set-face-foreground 'default "green")           ; normal text

;; enable visual feedback on selections
;(setq transient-mark-mode t)

;; default to better frame titles
(setq frame-title-format
      (concat  "%b - emacs@" system-name))
(c-set-offset 'substatement-open 0)


(setq c-basic-offset 4)                  ;; Default is 2
(setq c-indent-level 4)                  ;; Default is 2

(prefer-coding-system 'utf-8)
(setq coding-system-for-read 'utf-8)
(setq coding-system-for-write 'utf-8)

(custom-set-variables
 ;; custom-set-variables was added by Custom.
 ;; If you edit it by hand, you could mess it up, so be careful.
 ;; Your init file should contain only one such instance.
 ;; If there is more than one, they won't work right.
 '(auto-compression-mode t nil (jka-compr))
 '(c-objc-method-arg-min-delta-to-bracket 0)
 '(case-fold-search t)
 '(current-language-environment "Latin-1")
 '(default-input-method "latin-1-prefix")
 '(global-font-lock-mode t nil (font-lock))
 '(inhibit-startup-screen t))

;;(set-default-font "-adobe-courier-small-r-normal--12-180-75-75-m-110-iso8859-1")

(custom-set-faces
 ;; custom-set-faces was added by Custom.
 ;; If you edit it by hand, you could mess it up, so be careful.
 ;; Your init file should contain only one such instance.
 ;; If there is more than one, they won't work right.
 )

(setq-default indent-tabs-mode nil)
(set-face-attribute 'default nil :height 120)

;;setq auto-mode-alist (append '(("\\.h\\'" . c++-mode)) auto-mode-alist))

;;(setq load-home-init-file t) ; don't load init file from ~/.xemacs/init.el
(setq load-home-init-file t) ; don't load init file from ~/.xemacs/init.el

(global-set-key [C-mouse-4] 'text-scale-increase)
(global-set-key [C-mouse-5] 'text-scale-decrease)
(setq-default truncate-lines t)

(if (window-system)
  (set-frame-height (selected-frame) 50)
  )

(if (window-system)
    (set-frame-width  (selected-frame) 70)
  )
(setq default-frame-alist
      '((top . 30) (left . 500)
        (width . 130) (height . 50)))
(put 'upcase-region 'disabled nil)
