from django import template

register = template.Library()

@register.inclusion_tag('components/button.html')
def ui_button(label, url='#', extra_classes=''):
    """
    Renders a customizable button.
    :param label: Texto interno del botón
    :param url:  Destino (onclick)
    :param extra_classes: Clases CSS adicionales
    """
    return {
        'label': label,
        'url': url,
        'extra_classes': extra_classes,
    }

@register.inclusion_tag('components/card.html')
def ui_card(title=None, content='', footer=None, extra_classes=''):
    """
    Renders a card with optional title, content and footer.
    :param title: Encabezado de la tarjeta
    :param content: HTML o texto de cuerpo (ver nota de seguridad)
    :param footer: Pie opcional
    :param extra_classes: Clases CSS adicionales
    """
    return {
        'title': title,
        'content': content,
        'footer': footer,
        'extra_classes': extra_classes,
    }

@register.inclusion_tag('components/circle.html')
def ui_indicador(color, extra_classes=''):
    """
    Renderiza un círculo con uno de cinco colores:
    - red, green, yellow, blue, gray.
    Si color no es válido, usa 'gray' por defecto.
    """
    valid = {'red','green','yellow','blue','gray'}
    c = color if color in valid else 'gray'
    return {
        'color': c,
        'extra_classes': extra_classes,
    }